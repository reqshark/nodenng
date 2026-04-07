/*

  Copyright (c) 2019 Shiddeshwaran S <shiddeshwaran@gmail.com>

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

module.exports  = recv_msg

function recv_msg(t){
  t.plan(3)

  const str = 'recv succeeds for'
  const url = 'tcp://127.0.0.1'
  const {
    pair0_open,
    pair1_open,
    push_open,
    pull_open,
    dial,
    listen,
    send_msg,
    recv_msg,
    close,
  } = require('..')

  const message = Buffer.from('hello from sender');

  // pair0 - two sockets connected
  const pair0_a = pair0_open()
  const pair0_b = pair0_open()
  listen(pair0_a, `${url}:9878`)
  dial(pair0_b, `${url}:9878`)
  send_msg(pair0_b, message)
  const recv_pair0 = recv_msg(pair0_a)
  t.is(recv_pair0.toString(), message.toString(), `${str} pair0`)
  close(pair0_a)
  close(pair0_b)

  // pair1 - two sockets connected
  const pair1_a = pair1_open()
  const pair1_b = pair1_open()
  listen(pair1_a, `${url}:9879`)
  dial(pair1_b, `${url}:9879`)
  send_msg(pair1_b, message)
  const recv_pair1 = recv_msg(pair1_a)
  t.is(recv_pair1.toString(), message.toString(), `${str} pair1`)
  close(pair1_a)
  close(pair1_b)

  // push/pull
  const push = push_open()
  const pull = pull_open()
  listen(pull, `${url}:9882`)
  dial(push, `${url}:9882`)
  send_msg(push, message)
  const recv_pull = recv_msg(pull)
  t.is(recv_pull.toString(), message.toString(), `${str} pull`)
  close(push)
  close(pull)

  // Note: pub/sub requires delay for subscription to propagate
  // Note: req/rep/surveyor/respondent require proper request-reply exchange
}
