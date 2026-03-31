/*

  Copyright (c) 2026 Bent Cardan <bent@nothingsatisfies.com>

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

module.exports  = sendrecv

function sendrecv(t){
  t.plan(13)

  const {
    push_open,
    pull_open,
    req_open,
    rep_open,
    MAX_PAYLOAD,
    NNG_OPT_RECVTIMEO,
  } = require('..')

  t.is(MAX_PAYLOAD, 1492, 'MAX_PAYLOAD is 1492 (MTU-safe)')

  /* push/pull: sync send, async recv */
  const pull = pull_open()
  t.is(pull.listen('tcp://127.0.0.1:9701'), 0, 'pull listen')
  const push = push_open()
  t.is(push.dial('tcp://127.0.0.1:9701'), 0, 'push dial')
  t.is(push.send('hello'), 0, 'sync push send string')

  pull.recv(function(err, msg){
    t.is(err, null, 'async pull recv no error')
    t.is(msg.toString(), 'hello', 'async pull recv matches sent string')
    push.close()
    pull.close()

    /* req/rep: async send, async recv */
    const rep = rep_open()
    t.is(rep.listen('tcp://127.0.0.1:9702'), 0, 'rep listen')
    const req = req_open()
    req.setopt(NNG_OPT_RECVTIMEO, 1000)
    t.is(req.dial('tcp://127.0.0.1:9702'), 0, 'req dial')

    rep.recv(function(err, msg){
      t.is(err, null, 'async rep recv no error')
      t.is(msg.toString(), 'ping', 'async rep recv matches')
      rep.send(Buffer.from('pong'), function(err){
        t.is(err, null, 'async rep send no error')
      })
    })

    req.send(Buffer.from('ping'), function(err){
      t.is(err, null, 'async req send no error')
      req.recv(function(err, msg){
        t.is(msg.toString(), 'pong', 'async req recv matches reply')
        req.close()
        rep.close()
      })
    })
  })
}
