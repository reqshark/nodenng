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

module.exports  = nng_setopt

function nng_setopt(t){
  t.plan(5)

  const {
    NNG_OPT_RECVTIMEO,
    sub_open,
    req_open,
  } = require('..')

  t.is(NNG_OPT_RECVTIMEO, `recv-timeout`, 'NNG_OPT_RECVTIMEO symbol export succeeds')

  /* subscribe */
  const sub = sub_open()
  t.is(sub.subscribe(''), 0, 'sub socket subscribe succeeds')
  t.is(sub.close(), 0, 'closing the sub socket works')

  /* setopt/getopt */
  const req = req_open()
  t.is(req.setopt(NNG_OPT_RECVTIMEO, 1000), 0, 'setopt recv-timeout succeeds')
  t.is(req.close(), 0, 'closing the req socket works')
}
