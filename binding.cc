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

#include "timer.h"
#include "nan.h"
#include "ref.h"

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

NAN_METHOD(bus_open) {
  size_t sz = sizeof (nng_socket);
  nng_socket s;
  nng_bus0_open(&s); //nng_bus0_open_raw

  Local<Object> sock = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(sock), &s, sz);
  info.GetReturnValue().Set(sock);
}

NAN_METHOD(pair0_open) {
  size_t sz = sizeof (nng_socket);
  nng_socket s;
  nng_pair0_open(&s); //nng_pair0_open_raw

  Local<Object> sock = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(sock), &s, sz);
  info.GetReturnValue().Set(sock);
}

//NNG_OPT_PAIR1_POLY
//NNG_OPT_MAXTTL
NAN_METHOD(pair1_open) {
  size_t sz = sizeof (nng_socket);
  nng_socket s;
  nng_pair1_open(&s); //nng_pair1_open_raw

  Local<Object> sock = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(sock), &s, sz);
  info.GetReturnValue().Set(sock);
}

NAN_METHOD(pub_open) {
  size_t sz = sizeof (nng_socket);
  nng_socket s;
  nng_pub0_open(&s); //nng_pub0_open_raw

  Local<Object> sock = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(sock), &s, sz);
  info.GetReturnValue().Set(sock);
}

NAN_METHOD(pull_open) {
  size_t sz = sizeof (nng_socket);
  nng_socket s;
  nng_pull0_open(&s); //nng_pull0_open_raw

  Local<Object> sock = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(sock), &s, sz);
  info.GetReturnValue().Set(sock);
}

NAN_METHOD(push_open) {
  size_t sz = sizeof (nng_socket);
  nng_socket s;
  nng_push0_open(&s); //nng_push0_open_raw

  Local<Object> sock = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(sock), &s, sz);
  info.GetReturnValue().Set(sock);
}

NAN_METHOD(rep_open) {
  size_t sz = sizeof (nng_socket);
  nng_socket s;
  nng_rep0_open(&s); //nng_rep0_open_raw

  Local<Object> sock = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(sock), &s, sz);
  info.GetReturnValue().Set(sock);
}

NAN_METHOD(req_open) {
  size_t sz = sizeof (nng_socket);
  nng_socket s;
  nng_req0_open(&s); //nng_req0_open_raw

  Local<Object> sock = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(sock), &s, sz);
  info.GetReturnValue().Set(sock);
}

NAN_METHOD(respondent_open) {
  size_t sz = sizeof (nng_socket);
  nng_socket s;
  nng_respondent0_open(&s); //nng_respondent0_open_raw

  Local<Object> sock = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(sock), &s, sz);
  info.GetReturnValue().Set(sock);
}

NAN_METHOD(sub_open) {
  size_t sz = sizeof (nng_socket);
  nng_socket s;
  nng_sub0_open(&s); //nng_sub0_open_raw

  Local<Object> sock = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(sock), &s, sz);
  info.GetReturnValue().Set(sock);
}

NAN_METHOD(surveyor_open) {
  size_t sz = sizeof (nng_socket);
  nng_socket s;
  nng_surveyor0_open(&s); // nng_surveyor0_open_raw

  Local<Object> sock = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(sock), &s, sz);
  info.GetReturnValue().Set(sock);
}


/**
 * nanomsg.github.io/nng/man/tip/nng_close.3
 * Closing the socket while data is in transmission will likely lead to loss of
 * that data. There is no automatic linger or flush to ensure that the socket
 * send buffers have completely transmitted. It is recommended to wait a brief
 * period after calling nng_send() or similar functions, before calling this
 * function.
 */

NAN_METHOD(close) {
  int clsd = nng_close(*UnwrapPointer<nng_socket*>(info[0]));
  if (clsd == 0) {
    info.GetReturnValue().Set(New<Number>(clsd));
  } else {
    info.GetReturnValue().Set(New<String>(nng_strerror(clsd)).ToLocalChecked());
  }
}

//int nng_listen(nng_socket s, const char *url, nng_listener *lp, int flags);
NAN_METHOD(listen) {
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
      .Set(New<String>(nng_strerror(l)).ToLocalChecked());
  }
}

//int nng_dial(nng_socket s, const char *url, nng_dialer *dp, int flags);
NAN_METHOD(dial) {
  Utf8String url(info[1]);

  // do we want to return the nng_dialer?
  //nanomsg.github.io/nng/man/tip/nng_dialer.5
  int d = nng_dial(*UnwrapPointer<nng_socket*>(info[0]), *url, NULL, 0);

  if (d == 0) {
    info.GetReturnValue().Set(New<Number>(d)); // <-- good spot for nng_dialer
  } else {
    info.GetReturnValue().Set(New<String>(nng_strerror(d)).ToLocalChecked());
  }
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

NAN_MODULE_INIT(Init) {
  HandleScope scope;

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

  /* debug */
  EXPORT_METHOD(target, test);
}
NODE_MODULE(nodenng, Init)
