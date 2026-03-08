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

module.exports  = send_msg

function send_msg(t){
  t.plan(5)

  const str = 'send succeeds for'
  const url = 'tcp://127.0.0.1'
  const {
    bus_open,
    pair0_open,
    pair1_open,
    pub_open,
    push_open,
    pull_open,
    dial,
    listen,
    send_msg,
    close,
  } = require('..')

  const message = Buffer.from('hello from sender');

  // bus
  const bus = bus_open()
  listen(bus, `${url}:8877`)
  dial(bus, `${url}:8877`)
  t.is(send_msg(bus, message), 0, `${str} bus`)
  close(bus)

  // pair0
  const pair0 = pair0_open()
  listen(pair0, `${url}:8878`)
  dial(pair0, `${url}:8878`)
  t.is(send_msg(pair0, message), 0, `${str} pair0`)
  close(pair0)

  // pair1
  const pair1 = pair1_open()
  listen(pair1, `${url}:8879`)
  dial(pair1, `${url}:8879`)
  t.is(send_msg(pair1, message), 0, `${str} pair1`)
  close(pair1)

  // pub
  const pub = pub_open()
  dial(pub, `${url}:8880`)
  t.is(send_msg(pub, message), 0, `${str} pub`)
  close(pub)

  // push/pull
  const push = push_open()
  const pull = pull_open()
  listen(pull, `${url}:8882`)
  dial(push, `${url}:8882`)
  t.is(send_msg(push, message), 0, `${str} push`)
  close(push)
  close(pull)

  // Note: req/rep/surveyor/respondent require proper request-reply exchange
  // and cannot be tested for send without receiving first (or vice versa)
}
