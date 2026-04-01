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

module.exports  = close

function close(t){
  t.plan(11)

  const str = 'nng_close() succeeds for'

  const {
    bus_open,
    pair0_open,
    pair1_open,
    pub_open,
    pull_open,
    push_open,
    rep_open,
    req_open,
    respondent_open,
    sub_open,
    surveyor_open,
    close,
  } = require('..')

  const bus = bus_open()
  const pair0 = pair0_open()
  const pair1 = pair1_open()
  const pub = pub_open()
  const pull = pull_open()
  const push = push_open()
  const rep = rep_open()
  const req = req_open()
  const respondent = respondent_open()
  const sub = sub_open()
  const surveyor = surveyor_open()

  t.is(close(bus), 0, `${str} bus`)
  t.is(close(pair0), 0, `${str} pair0`)
  t.is(close(pair1), 0, `${str} pair1`)
  t.is(close(pub), 0, `${str} pub`)
  t.is(close(pull), 0, `${str} pull`)
  t.is(close(push), 0, `${str} push`)
  t.is(close(rep), 0, `${str} rep`)
  t.is(close(req), 0, `${str} req`)
  t.is(close(respondent), 0, `${str} respondent`)
  t.is(close(sub), 0, `${str} sub`)
  t.is(close(surveyor), 0, `${str} surveyor`)
}
