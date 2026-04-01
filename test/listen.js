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

module.exports  = listen

function listen(t){
  t.plan(11)

  const str = 'listen succeeds for'
  const url = 'tcp://127.0.0.1'
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
    listen,
  } = require('..')

  t.is(listen(bus_open(), `${url}:7777`), 0, `${str} bus`)
  t.is(listen(pair0_open(), `${url}:7778`), 0, `${str} pair0`)
  t.is(listen(pair1_open(), `${url}:7779`), 0, `${str} pair1`)
  t.is(listen(pub_open(), `${url}:7780`), 0, `${str} pub`)
  t.is(listen(pull_open(), `${url}:7781`), 0, `${str} pull`)
  t.is(listen(push_open(), `${url}:7782`), 0, `${str} push`)
  t.is(listen(rep_open(), `${url}:7783`), 0, `${str} rep`)
  t.is(listen(req_open(), `${url}:7784`), 0, `${str} req`)
  t.is(listen(respondent_open(), `${url}:7785`), 0, `${str} respondent`)
  t.is(listen(sub_open(), `${url}:7786`), 0, `${str} sub`)
  t.is(listen(surveyor_open(), `${url}:7787`), 0, `${str} surveyor`)
}
