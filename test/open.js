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

  t.is (typeof bus_open().id, 'number', 'open bus returns a socket with id')
  t.is (typeof pair0_open().id, 'number', 'open pair0 returns a socket with id')
  t.is (typeof pair1_open().id, 'number', 'open pair1 returns a socket with id')
  t.is (typeof pub_open().id, 'number', 'open pub returns a socket with id')
  t.is (typeof pull_open().id, 'number', 'open pull returns a socket with id')
  t.is (typeof push_open().id, 'number', 'open push returns a socket with id')
  t.is (typeof rep_open().id, 'number', 'open rep returns a socket with id')
  t.is (typeof req_open().id, 'number', 'open req returns a socket with id')
  t.is (typeof respondent_open().id, 'number', 'open respondent returns a socket with id')
  t.is (typeof sub_open().id, 'number', 'open sub returns a socket with id')
  t.is (typeof surveyor_open().id, 'number', 'open surveyor returns a socket with id')

}
