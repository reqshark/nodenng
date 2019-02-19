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

module.exports  = open

function open(t){
  t.plan(11)

  const { isBuffer } = Buffer
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
  } = require('..')

  t.ok ( isBuffer(bus_open()), 'open bus sock is a node buffer')
  t.ok ( isBuffer(pair0_open()), 'open pair0 sock is a node buffer')
  t.ok ( isBuffer(pair1_open()), 'open pair1 sock is a node buffer')
  t.ok ( isBuffer(pub_open()), 'open pub sock is a node buffer')
  t.ok ( isBuffer(pull_open()), 'open pull sock is a node buffer')
  t.ok ( isBuffer(push_open()), 'open push sock is a node buffer')
  t.ok ( isBuffer(rep_open()), 'open rep sock is a node buffer')
  t.ok ( isBuffer(req_open()), 'open req sock is a node buffer')
  t.ok ( isBuffer(respondent_open()), 'open respondent sock is a node buffer')
  t.ok ( isBuffer(sub_open()), 'open sub sock is a node buffer')
  t.ok ( isBuffer(surveyor_open()), 'open surveyor sock is a node buffer')

}
