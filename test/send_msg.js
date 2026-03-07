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
  t.plan(9)

  const str = 'send succeeds for'
  const url = 'tcp://127.0.0.1'
  const {
    bus_open,
    pair0_open,
    pair1_open,
    pub_open,
    push_open,
    rep_open,
    req_open,
    respondent_open,
    surveyor_open,
    dial,
    send_msg,
  } = require('..')

  const message = Buffer.from('hello from sender');

  // bus
  const bus = bus_open()
  dial(bus, `${url}:7777`)
  t.is(send_msg(bus, message), 0, `${str} bus`)

  // pair0
  const pair0 = pair0_open()
  dial(pair0, `${url}:7778`)
  t.is(send_msg(pair0, message), 0, `${str} pair0`)

  // pair1
  const pair1 = pair1_open()
  dial(pair1, `${url}:7779`)
  t.is(send_msg(pair1, message), 0, `${str} pair1`)

  // pub
  const pub = pub_open()
  dial(pub, `${url}:7780`)
  t.is(send_msg(pub, message), 0, `${str} pub`)

  // push
  const push = push_open()
  dial(push, `${url}:7782`)
  t.is(send_msg(push, message), 0, `${str} push`)

  // rep
  const rep = rep_open()
  dial(rep, `${url}:7783`)
  t.is(send_msg(rep, message), 0, `${str} rep`)

  // req
  const req = req_open()
  dial(req, `${url}:7784`)
  t.is(send_msg(req, message), 0, `${str} req`)

  // respondent
  const respondent = respondent_open()
  dial(respondent, `${url}:7785`)
  t.is(send_msg(respondent, message), 0, `${str} respondent`)

  // surveyor
  const surveyor = surveyor_open()
  dial(surveyor, `${url}:7787`)
  t.is(send_msg(surveyor, message), 0, `${str} surveyor`)
}
