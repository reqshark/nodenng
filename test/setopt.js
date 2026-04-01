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
  t.plan(3)

  const {
    NNG_OPT_SUB_SUBSCRIBE,
    sub_open,
    setopt,
    close,
  } = require('..')

  t.is(NNG_OPT_SUB_SUBSCRIBE, `sub:subscribe`, 'library symbol export succeeds')

  const topic = '' /* subscription topic all */
  const sub = sub_open()
  const r = setopt(sub, NNG_OPT_SUB_SUBSCRIBE, topic)

  t.is(r, 0, 'sub socket configured subscription option succeeds')
  t.is(close(sub), 0, 'closing the sub socket works')
}
