/*

  Copyright (c) 2026 Bent Cardan <bent@nothingsatisfies.com>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom
  the Software is furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/

#include <nng/nng.h>
#include <nan.h>
#include <string>

class SendWorker : public Nan::AsyncWorker {
 public:
  SendWorker(Nan::Callback *cb, nng_socket sock, std::string data)
    : Nan::AsyncWorker(cb), sock(sock), data(std::move(data)) {}

  void Execute() override {
    int rv = nng_send(sock, (void *)data.data(), data.size(), 0);
    if (rv != 0) SetErrorMessage(nng_strerror((nng_err)rv));
  }

  void HandleOKCallback() override {
    Nan::HandleScope scope;
    v8::Local<v8::Value> argv[] = { Nan::Null() };
    callback->Call(1, argv, async_resource);
  }

 private:
  nng_socket sock;
  std::string data;
};

class RecvWorker : public Nan::AsyncWorker {
 public:
  RecvWorker(Nan::Callback *cb, nng_socket sock)
    : Nan::AsyncWorker(cb), sock(sock), msg(NULL) {}

  ~RecvWorker() { if (msg) nng_msg_free(msg); }

  void Execute() override {
    int rv = nng_recvmsg(sock, &msg, 0);
    if (rv != 0) SetErrorMessage(nng_strerror((nng_err)rv));
  }

  void HandleOKCallback() override {
    Nan::HandleScope scope;
    size_t len = nng_msg_len(msg);
    v8::Local<v8::Value> argv[] = {
      Nan::Null(),
      Nan::CopyBuffer((char *)nng_msg_body(msg), len).ToLocalChecked()
    };
    nng_msg_free(msg);
    msg = NULL;
    callback->Call(2, argv, async_resource);
  }

 private:
  nng_socket sock;
  nng_msg *msg;
};

class Socket : public Nan::ObjectWrap {
 public:
  nng_socket sock;

  static NAN_MODULE_INIT(Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Socket").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "close", Close);
    Nan::SetPrototypeMethod(tpl, "listen", Listen);
    Nan::SetPrototypeMethod(tpl, "dial", Dial);
    Nan::SetPrototypeMethod(tpl, "send", Send);
    Nan::SetPrototypeMethod(tpl, "recv", Recv);
    Nan::SetPrototypeMethod(tpl, "setopt", Setopt);
    Nan::SetPrototypeMethod(tpl, "getopt", Getopt);
    Nan::SetPrototypeMethod(tpl, "subscribe", Subscribe);
    Nan::SetPrototypeMethod(tpl, "unsubscribe", Unsubscribe);

    Nan::SetAccessor(tpl->InstanceTemplate(),
      Nan::New("id").ToLocalChecked(), GetId);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
  }

  // factory: create a Socket JS object from an already-opened nng_socket
  static v8::Local<v8::Object> NewInstance(nng_socket s) {
    v8::Local<v8::Function> cons = Nan::New(constructor());
    v8::Local<v8::Object> instance =
      Nan::NewInstance(cons, 0, NULL).ToLocalChecked();
    Socket *obj = Nan::ObjectWrap::Unwrap<Socket>(instance);
    obj->sock = s;
    return instance;
  }

 private:
  Socket() { sock.id = 0; }

  ~Socket() {
    if (sock.id != 0) {
      nng_socket_close(sock);
      sock.id = 0;
    }
  }

  static NAN_METHOD(New) {
    if (info.IsConstructCall()) {
      Socket *obj = new Socket();
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    }
  }

  static NAN_GETTER(GetId) {
    Socket *obj = Nan::ObjectWrap::Unwrap<Socket>(info.This());
    info.GetReturnValue().Set(Nan::New<v8::Number>(obj->sock.id));
  }

  static NAN_METHOD(Close) {
    Socket *obj = Nan::ObjectWrap::Unwrap<Socket>(info.Holder());
    int rv = nng_socket_close(obj->sock);
    obj->sock.id = 0;
    if (rv == 0) {
      info.GetReturnValue().Set(Nan::New<v8::Number>(rv));
    } else {
      info.GetReturnValue().Set(
        Nan::New<v8::String>(nng_strerror((nng_err)rv)).ToLocalChecked());
    }
  }

  static NAN_METHOD(Listen) {
    Socket *obj = Nan::ObjectWrap::Unwrap<Socket>(info.Holder());
    Nan::Utf8String url(info[0]);
    int rv = nng_listen(obj->sock, *url, NULL, 0);
    if (rv == 0) {
      info.GetReturnValue().Set(Nan::New<v8::Number>(rv));
    } else {
      info.GetReturnValue().Set(
        Nan::New<v8::String>(nng_strerror((nng_err)rv)).ToLocalChecked());
    }
  }

  static NAN_METHOD(Dial) {
    Socket *obj = Nan::ObjectWrap::Unwrap<Socket>(info.Holder());
    Nan::Utf8String url(info[0]);
    int rv = nng_dial(obj->sock, *url, NULL, 0);
    if (rv == 0) {
      info.GetReturnValue().Set(Nan::New<v8::Number>(rv));
    } else {
      info.GetReturnValue().Set(
        Nan::New<v8::String>(nng_strerror((nng_err)rv)).ToLocalChecked());
    }
  }

  static NAN_METHOD(Setopt) {
    Socket *obj = Nan::ObjectWrap::Unwrap<Socket>(info.Holder());
    Nan::Utf8String opt(info[0]);
    int rv;

    if (info[1]->IsBoolean()) {
      bool val = Nan::To<bool>(info[1]).FromJust();
      rv = nng_socket_set_bool(obj->sock, *opt, val);
    } else if (info[1]->IsNumber()) {
      int val = Nan::To<int>(info[1]).FromJust();
      rv = nng_socket_set_int(obj->sock, *opt, val);
      if (rv == NNG_EBADTYPE) {
        rv = nng_socket_set_ms(obj->sock, *opt, (nng_duration)val);
      }
      if (rv == NNG_EBADTYPE) {
        rv = nng_socket_set_size(obj->sock, *opt, (size_t)val);
      }
    } else {
      rv = NNG_EINVAL;
    }

    if (rv == 0) {
      info.GetReturnValue().Set(Nan::New<v8::Number>(rv));
    } else {
      info.GetReturnValue().Set(
        Nan::New<v8::String>(nng_strerror((nng_err)rv)).ToLocalChecked());
    }
  }

  static NAN_METHOD(Getopt) {
    Socket *obj = Nan::ObjectWrap::Unwrap<Socket>(info.Holder());
    Nan::Utf8String opt(info[0]);
    int rv;

    // try each typed getter until one succeeds
    { bool val; rv = nng_socket_get_bool(obj->sock, *opt, &val);
      if (rv == 0) { info.GetReturnValue().Set(Nan::New<v8::Boolean>(val)); return; } }
    { int val; rv = nng_socket_get_int(obj->sock, *opt, &val);
      if (rv == 0) { info.GetReturnValue().Set(Nan::New<v8::Number>(val)); return; } }
    { nng_duration val; rv = nng_socket_get_ms(obj->sock, *opt, &val);
      if (rv == 0) { info.GetReturnValue().Set(Nan::New<v8::Number>(val)); return; } }
    { size_t val; rv = nng_socket_get_size(obj->sock, *opt, &val);
      if (rv == 0) { info.GetReturnValue().Set(Nan::New<v8::Number>((double)val)); return; } }

    info.GetReturnValue().Set(
      Nan::New<v8::String>(nng_strerror((nng_err)rv)).ToLocalChecked());
  }

  static NAN_METHOD(Subscribe) {
    Socket *obj = Nan::ObjectWrap::Unwrap<Socket>(info.Holder());
    const char *topic = "";
    size_t topic_sz = 0;
    Nan::Utf8String topicStr(info[0]);
    if (info[0]->IsString()) {
      topic = *topicStr;
      topic_sz = strlen(topic);
    }
    int rv = nng_sub0_socket_subscribe(obj->sock, topic, topic_sz);
    if (rv == 0) {
      info.GetReturnValue().Set(Nan::New<v8::Number>(rv));
    } else {
      info.GetReturnValue().Set(
        Nan::New<v8::String>(nng_strerror((nng_err)rv)).ToLocalChecked());
    }
  }

  static NAN_METHOD(Send) {
    Socket *obj = Nan::ObjectWrap::Unwrap<Socket>(info.Holder());
    std::string copy;

    if (node::Buffer::HasInstance(info[0])) {
      copy.assign(node::Buffer::Data(info[0]), node::Buffer::Length(info[0]));
    } else if (info[0]->IsString()) {
      Nan::Utf8String str(info[0]);
      copy.assign(*str, str.length());
    } else {
      return Nan::ThrowTypeError("send() requires a String or Buffer");
    }

    /* async: send(data, callback) */
    if (info.Length() > 1 && info[1]->IsFunction()) {
      Nan::Callback *cb = new Nan::Callback(info[1].As<v8::Function>());
      Nan::AsyncQueueWorker(new SendWorker(cb, obj->sock, std::move(copy)));
      return;
    }

    /* sync: send(data) */
    int rv = nng_send(obj->sock, (void *)copy.data(), copy.size(), 0);
    if (rv == 0) {
      info.GetReturnValue().Set(Nan::New<v8::Number>(rv));
    } else {
      info.GetReturnValue().Set(
        Nan::New<v8::String>(nng_strerror((nng_err)rv)).ToLocalChecked());
    }
  }

  static NAN_METHOD(Recv) {
    Socket *obj = Nan::ObjectWrap::Unwrap<Socket>(info.Holder());

    if (!info[0]->IsFunction()) {
      return Nan::ThrowTypeError("recv() requires a callback");
    }

    Nan::Callback *cb = new Nan::Callback(info[0].As<v8::Function>());
    Nan::AsyncQueueWorker(new RecvWorker(cb, obj->sock));
  }

  static NAN_METHOD(Unsubscribe) {
    Socket *obj = Nan::ObjectWrap::Unwrap<Socket>(info.Holder());
    const char *topic = "";
    size_t topic_sz = 0;
    Nan::Utf8String topicStr(info[0]);
    if (info[0]->IsString()) {
      topic = *topicStr;
      topic_sz = strlen(topic);
    }
    int rv = nng_sub0_socket_unsubscribe(obj->sock, topic, topic_sz);
    if (rv == 0) {
      info.GetReturnValue().Set(Nan::New<v8::Number>(rv));
    } else {
      info.GetReturnValue().Set(
        Nan::New<v8::String>(nng_strerror((nng_err)rv)).ToLocalChecked());
    }
  }

  static inline Nan::Persistent<v8::Function> & constructor() {
    static Nan::Persistent<v8::Function> ctor;
    return ctor;
  }
};

// open functions — each returns a Socket JS object
#define OPEN_METHOD(name, fn)                                                  \
NAN_METHOD(name) {                                                             \
  nng_socket s;                                                                \
  int rv = fn(&s);                                                             \
  if (rv == 0) {                                                               \
    info.GetReturnValue().Set(Socket::NewInstance(s));                          \
  } else {                                                                     \
    Nan::ThrowError(nng_strerror((nng_err)rv));                                \
  }                                                                            \
}

OPEN_METHOD(bus_open,        nng_bus0_open)
OPEN_METHOD(pair0_open,      nng_pair0_open)
OPEN_METHOD(pair1_open,      nng_pair1_open)
OPEN_METHOD(pub_open,        nng_pub0_open)
OPEN_METHOD(pull_open,       nng_pull0_open)
OPEN_METHOD(push_open,       nng_push0_open)
OPEN_METHOD(rep_open,        nng_rep0_open)
OPEN_METHOD(req_open,        nng_req0_open)
OPEN_METHOD(respondent_open, nng_respondent0_open)
OPEN_METHOD(sub_open,        nng_sub0_open)
OPEN_METHOD(surveyor_open,   nng_surveyor0_open)

#define EXPORT_METHOD(C, S)                                                    \
  Nan::Set(C, Nan::New(#S).ToLocalChecked(),                                   \
    Nan::GetFunction(Nan::New<v8::FunctionTemplate>(S)).ToLocalChecked());

#define EXPORT_SYMBOL(C, S)                                                    \
  Nan::Set(C, Nan::New(#S).ToLocalChecked(), Nan::New(S).ToLocalChecked());

NAN_MODULE_INIT(Init) {
  nng_init(NULL);

  Socket::Init(target);

  /* open */
  EXPORT_METHOD(target, bus_open);
  EXPORT_METHOD(target, pair0_open);
  EXPORT_METHOD(target, pair1_open);
  EXPORT_METHOD(target, pub_open);
  EXPORT_METHOD(target, pull_open);
  EXPORT_METHOD(target, push_open);
  EXPORT_METHOD(target, rep_open);
  EXPORT_METHOD(target, req_open);
  EXPORT_METHOD(target, respondent_open);
  EXPORT_METHOD(target, sub_open);
  EXPORT_METHOD(target, surveyor_open);

  /* NNG_OPT symbols */
  EXPORT_SYMBOL(target, NNG_OPT_RECVBUF);
  EXPORT_SYMBOL(target, NNG_OPT_SENDBUF);
  EXPORT_SYMBOL(target, NNG_OPT_RECVTIMEO);
  EXPORT_SYMBOL(target, NNG_OPT_SENDTIMEO);
  EXPORT_SYMBOL(target, NNG_OPT_MAXTTL);
  EXPORT_SYMBOL(target, NNG_OPT_RECVMAXSZ);
  EXPORT_SYMBOL(target, NNG_OPT_RECONNMINT);
  EXPORT_SYMBOL(target, NNG_OPT_RECONNMAXT);
  EXPORT_SYMBOL(target, NNG_OPT_TCP_NODELAY);
  EXPORT_SYMBOL(target, NNG_OPT_TCP_KEEPALIVE);
  EXPORT_SYMBOL(target, NNG_OPT_PAIR1_POLY);
  EXPORT_SYMBOL(target, NNG_OPT_SUB_PREFNEW);
  EXPORT_SYMBOL(target, NNG_OPT_REQ_RESENDTIME);
  EXPORT_SYMBOL(target, NNG_OPT_REQ_RESENDTICK);
  EXPORT_SYMBOL(target, NNG_OPT_SURVEYOR_SURVEYTIME);

  /* MTU-safe payload size: 1500 MTU minus IP header overhead */
  Nan::Set(target, Nan::New("MAX_PAYLOAD").ToLocalChecked(),
    Nan::New<v8::Number>(1492));
}
NODE_MODULE(nodenng, Init)
