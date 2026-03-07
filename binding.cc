/*

  Copyright (c) 2019 Bent Cardan <bent@nothingsatisfies.com>

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

#if !defined(NNG_MAJOR_VERSION) || (NNG_MAJOR_VERSION < 2)
#include <nng/protocol/bus0/bus.h>
#include <nng/protocol/pair0/pair.h>
#include <nng/protocol/pair1/pair.h>
#include <nng/protocol/pubsub0/pub.h>
#include <nng/protocol/pipeline0/pull.h>
#include <nng/protocol/pipeline0/push.h>
#include <nng/protocol/reqrep0/rep.h>
#include <nng/protocol/reqrep0/req.h>
#include <nng/protocol/survey0/respond.h>
#include <nng/protocol/pubsub0/sub.h>
#include <nng/protocol/survey0/survey.h>
#endif

#include "timer.h"
#include "nan.h"
#include "ref.h"
#include <cstring>

#if defined(NNG_MAJOR_VERSION) && (NNG_MAJOR_VERSION >= 2)
#define NODENNG_CLOSE(s) nng_socket_close(s)
#define NODENNG_STRERROR(e) nng_strerror(static_cast<nng_err>(e))
#else
#define NODENNG_CLOSE(s) nng_close(s)
#define NODENNG_STRERROR(e) nng_strerror(e)
#endif

using v8::FunctionTemplate;
using v8::Function;
using v8::Number;
using v8::Boolean;
using v8::String;
using v8::Object;
using v8::Value;
using v8::Local;

using Nan::HandleScope;
using Nan::MaybeLocal;
using Nan::Utf8String;
using Nan::NewBuffer;
using Nan::Callback;
using Nan::Maybe;
using Nan::Set;
using Nan::New;
using Nan::To;

#define DEFINE_OPENER(name, func) \
NAN_METHOD(name) { \
  size_t sz = sizeof(nng_socket); \
  nng_socket s = {}; \
  int rv = func(&s); \
  if (rv != 0) { \
    Nan::ThrowError(NODENNG_STRERROR(rv)); \
    return; \
  } \
  Local<Object> sock = NewBuffer(sz).ToLocalChecked(); \
  memcpy(node::Buffer::Data(sock), &s, sz); \
  info.GetReturnValue().Set(sock); \
}

DEFINE_OPENER(bus_open, nng_bus0_open)
DEFINE_OPENER(pair0_open, nng_pair0_open)
DEFINE_OPENER(pair1_open, nng_pair1_open)
DEFINE_OPENER(pub_open, nng_pub0_open)
DEFINE_OPENER(sub_open, nng_sub0_open)
DEFINE_OPENER(pull_open, nng_pull0_open)
DEFINE_OPENER(push_open, nng_push0_open)
DEFINE_OPENER(req_open, nng_req0_open)
DEFINE_OPENER(rep_open, nng_rep0_open)
DEFINE_OPENER(surveyor_open, nng_surveyor0_open)
DEFINE_OPENER(respondent_open, nng_respondent0_open)

/**
 * nanomsg.github.io/nng/man/tip/nng_close.3
 * Closing the socket while data is in transmission will likely lead to loss of
 * that data. There is no automatic linger or flush to ensure that the socket
 * send buffers have completely transmitted. It is recommended to wait a brief
 * period after calling nng_send() or similar functions, before calling this
 * function.
 */

NAN_METHOD(close) {
  if (info.Length() < 1) {
    Nan::ThrowTypeError("Socket required");
    return;
  }

  int clsd = NODENNG_CLOSE(*UnwrapPointer<nng_socket*>(info[0]));
  if (clsd == 0) {
    info.GetReturnValue().Set(New<Number>(clsd));
  } else {
    info.GetReturnValue().Set(New<String>(NODENNG_STRERROR(clsd)).ToLocalChecked());
  }
}

//int nng_listen(nng_socket s, const char *url, nng_listener *lp, int flags);
NAN_METHOD(listen) {
  if (info.Length() < 2) {
    Nan::ThrowTypeError("Socket and URL required");
    return;
  }

  Utf8String url(info[1]);

  // do we want to return the nng_listener??
  // nanomsg.github.io/nng/man/tip/nng_listener.5
  int l = nng_listen(*UnwrapPointer<nng_socket*>(info[0]), *url, NULL, 0);

  if (l == 0) {
    info
      .GetReturnValue()
      .Set(New<Number>(l)); // <-- good spot for nng_listener
  } else {
    info
      .GetReturnValue()
      .Set(New<String>(NODENNG_STRERROR(l)).ToLocalChecked());
  }
}

//int nng_dial(nng_socket s, const char *url, nng_dialer *dp, int flags);
NAN_METHOD(dial) {
  if (info.Length() < 2) {
    Nan::ThrowTypeError("Socket and URL required");
    return;
  }

  Utf8String url(info[1]);

  // do we want to return the nng_dialer?
  //nanomsg.github.io/nng/man/tip/nng_dialer.5
  int d = nng_dial(*UnwrapPointer<nng_socket*>(info[0]), *url, NULL, 0);

  if (d == 0) {
    info.GetReturnValue().Set(New<Number>(d)); // <-- good spot for nng_dialer
  } else {
    info.GetReturnValue().Set(New<String>(NODENNG_STRERROR(d)).ToLocalChecked());
  }
}


/**
 * https://nanomsg.github.io/nng/man/tip/nng_setopt.3
 *
 * int nng_setopt(nng_socket s, const char *opt, const void *val, size_t valsz);
 *
 * used to configure options for the socket s
 */

NAN_METHOD(setopt) {
  if (info.Length() < 3) {
    Nan::ThrowTypeError("Socket, option, and value required");
    return;
  }

  Utf8String opt(info[1]);
  Utf8String val(info[2]);
  int sopt;

#if defined(NNG_MAJOR_VERSION) && (NNG_MAJOR_VERSION >= 2)
  sopt = NNG_ENOTSUP;
  if (strcmp(*opt, "sub:subscribe") == 0) {
    sopt = nng_sub0_socket_subscribe(
      *UnwrapPointer<nng_socket*>(info[0]),
      *val,
      strlen(*val));
  }
#else
  sopt = nng_setopt(
    *UnwrapPointer<nng_socket*>(info[0]),
    *opt,
    *val,
    strlen(*val));
#endif

  if (sopt == 0) {
    info.GetReturnValue().Set(New<Number>(sopt));
  } else {
    info.GetReturnValue().Set(New<String>(NODENNG_STRERROR(sopt)).ToLocalChecked());
  }
}

NAN_METHOD(send_msg) {
  if (info.Length() < 2) {
    Nan::ThrowTypeError("Socket and message required");
    return;
  }

  nng_socket *sock = UnwrapPointer<nng_socket*>(info[0]);

  if (!node::Buffer::HasInstance(info[1])) {
    Nan::ThrowTypeError("Argument must be a Buffer");
    return;
  }

  char *data = node::Buffer::Data(info[1]);
  size_t len = node::Buffer::Length(info[1]);

  int rv = nng_send(*sock, data, len, 0);

  if (rv == 0) {
    info.GetReturnValue().Set(New<Number>(rv));
  } else {
    info.GetReturnValue().Set(
      New<String>(NODENNG_STRERROR(rv)).ToLocalChecked()
    );
  }
}

NAN_METHOD(recv_msg) {
  if (info.Length() < 1) {
    Nan::ThrowTypeError("Socket required");
    return;
  }

  nng_socket *sock = UnwrapPointer<nng_socket*>(info[0]);

#if defined(NNG_MAJOR_VERSION) && (NNG_MAJOR_VERSION >= 2)

  nng_msg *msg = NULL;
  int rv = nng_recvmsg(*sock, &msg, 0);

  if (rv != 0) {
    info.GetReturnValue().Set(
      New<String>(NODENNG_STRERROR(rv)).ToLocalChecked()
    );
    return;
  }

  void *buf = nng_msg_body(msg);
  size_t size = nng_msg_len(msg);

  Local<Object> out =
    Nan::CopyBuffer((char*)buf, size).ToLocalChecked();

  nng_msg_free(msg);
#else

  void *buf = NULL;
  size_t size;

  int rv = nng_recv(*sock, &buf, &size, NNG_FLAG_ALLOC);

  if (rv != 0) {
    info.GetReturnValue().Set(
      New<String>(NODENNG_STRERROR(rv)).ToLocalChecked()
    );
    return;
  }

  Local<Object> out =
    Nan::CopyBuffer((char*)buf, size).ToLocalChecked();

  nng_free(buf, size);
#endif

  info.GetReturnValue().Set(out);
}

NAN_METHOD(test){
  printf("sleeping for 5\n");
  reqsleep(5);
  printf("done sleeping\n");
  return;
}


#define EXPORT_METHOD(C, S)                                                    \
  Set(C, New(#S).ToLocalChecked(),                                             \
    Nan::GetFunction(New<FunctionTemplate>(S)).ToLocalChecked());

#define EXPORT_SYMBOL(C, S)                                                    \
  Set(C, New(#S).ToLocalChecked(), New(S).ToLocalChecked());                   \

NAN_MODULE_INIT(Init) {
  HandleScope scope;

#if defined(NNG_MAJOR_VERSION) && (NNG_MAJOR_VERSION >= 2)
  nng_err init_rv = nng_init(NULL);
  if (init_rv != 0) {
    Nan::ThrowError(NODENNG_STRERROR(init_rv));
    return;
  }
#endif

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

  /* close */
  EXPORT_METHOD(target, close);

  /* listen and dial */
  EXPORT_METHOD(target, listen);
  EXPORT_METHOD(target, dial);

  /* setopt */
  EXPORT_METHOD(target, setopt);

  /* send and recv */
  EXPORT_METHOD(target, send_msg);
  EXPORT_METHOD(target, recv_msg);

  /* debug */
  EXPORT_METHOD(target, test);

  /* symbols */
#if defined(NNG_OPT_SUB_SUBSCRIBE)
  EXPORT_SYMBOL(target, NNG_OPT_SUB_SUBSCRIBE);
#else
  Set(
    target,
    New("NNG_OPT_SUB_SUBSCRIBE").ToLocalChecked(),
    New("sub:subscribe").ToLocalChecked());
#endif
#if defined(NNG_OPT_SUB_UNSUBSCRIBE)
  EXPORT_SYMBOL(target, NNG_OPT_SUB_UNSUBSCRIBE);
#else
  Set(
    target,
    New("NNG_OPT_SUB_UNSUBSCRIBE").ToLocalChecked(),
    New("sub:unsubscribe").ToLocalChecked());
#endif
}
NODE_MODULE(nodenng, Init)
