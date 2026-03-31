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

module.exports  = pubsub

function pubsub(t){
  t.plan(7)

  const {
    pub_open,
    sub_open,
    NNG_OPT_SENDTIMEO,
    NNG_OPT_RECVTIMEO,
  } = require('..')

  const pub = pub_open()
  t.is(pub.listen('tcp://127.0.0.1:9710'), 0, 'pub listen')
  pub.setopt(NNG_OPT_SENDTIMEO, 1000)

  /* sub_open() auto-subscribes to all by default */
  const sub1 = sub_open()
  sub1.setopt(NNG_OPT_RECVTIMEO, 1000)
  t.is(sub1.dial('tcp://127.0.0.1:9710'), 0, 'sub1 dial')

  /* sub_open('hi') auto-subscribes to topic prefix */
  const sub2 = sub_open('hi')
  sub2.setopt(NNG_OPT_RECVTIMEO, 1000)
  t.is(sub2.dial('tcp://127.0.0.1:9710'), 0, 'sub2 dial')

  setTimeout(function(){
    pub.send('hi world', function(err){
      t.is(err, null, 'pub send succeeds')
    })

    sub1.recv(function(err, msg){
      t.is(msg.toString(), 'hi world', 'sub1 receives (subscribed all)')
      sub1.close()
    })

    sub2.recv(function(err, msg){
      t.is(msg.toString(), 'hi world', 'sub2 receives (topic prefix match)')
      sub2.close()

      /* unsubscribe still works */
      const sub3 = sub_open()
      sub3.setopt(NNG_OPT_RECVTIMEO, 200)
      sub3.dial('tcp://127.0.0.1:9710')
      sub3.unsubscribe('')
      sub3.recv(function(err){
        t.ok(err, 'unsubscribed sub3 recv times out')
        sub3.close()
        pub.close()
      })
    })
  }, 100)
}
