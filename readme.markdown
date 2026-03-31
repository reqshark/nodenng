# nodenng [![CircleCI](https://circleci.com/gh/reqshark/nodenng/tree/master.svg?style=svg)](https://circleci.com/gh/reqshark/nodenng/tree/master)

bindings to nng (nanomsg next generation)

# install
```bash
# npm i nodenng
git clone https://github.com/reqshark/nodenng.git
cd nodenng
npm i
npm t
```
installation note: `npm i nodenng` is on hold. still working on getting my npm account back because I cant login to do npm publish yet. I will sort that out and make it possible to do `npm i` again once I can log back in.

# use

### req/rep
```js
const { req_open, rep_open, NNG_OPT_RECVTIMEO } = require('nodenng')

const rep = rep_open()
rep.listen('tcp://127.0.0.1:3000')

const req = req_open()
req.setopt(NNG_OPT_RECVTIMEO, 1000)
req.dial('tcp://127.0.0.1:3000')

rep.recv(function(err, msg){
  console.log('received:', msg.toString())
  rep.send('world')
})

req.send('hello', function(err){
  req.recv(function(err, msg){
    console.log('reply:', msg.toString())
    req.close()
    rep.close()
  })
})
```

### pub/sub
```js
const { pub_open, sub_open } = require('nodenng')

const pub = pub_open()
pub.listen('tcp://127.0.0.1:3001')

const sub = sub_open()
sub.subscribe('')
sub.dial('tcp://127.0.0.1:3001')

sub.recv(function(err, msg){
  console.log('got:', msg.toString())
  sub.close()
  pub.close()
})

setTimeout(function(){ pub.send('hello') }, 100)
```

### push/pull
```js
const { push_open, pull_open } = require('nodenng')

const pull = pull_open()
pull.listen('tcp://127.0.0.1:3002')

const push = push_open()
push.dial('tcp://127.0.0.1:3002')
push.send('work item')

pull.recv(function(err, msg){
  console.log('pulled:', msg.toString())
  push.close()
  pull.close()
})
```

# api

### socket methods
- `sock.send(data [, callback])` — send string or buffer (sync without callback, async with)
- `sock.recv(callback)` — async recv, callback fn receives `(err, buf)`
- `sock.listen(url)` — bind to address
- `sock.dial(url)` — connect to address
- `sock.subscribe(topic)` — subscribe (sub sockets)
- `sock.unsubscribe(topic)` — unsubscribe (sub sockets)
- `sock.setopt(opt, val)` — set socket option
- `sock.getopt(opt)` — get socket option
- `sock.close()` — close socket
- `sock.id` — socket id (readonly)

### constants
- `MAX_PAYLOAD` — 1492 (MTU-safe payload size)
- `NNG_OPT_RECVTIMEO`, `NNG_OPT_SENDTIMEO`, `NNG_OPT_RECVBUF`, `NNG_OPT_SENDBUF`, `NNG_OPT_RECVMAXSZ`, `NNG_OPT_MAXTTL`, `NNG_OPT_RECONNMINT`, `NNG_OPT_RECONNMAXT`, `NNG_OPT_TCP_NODELAY`, `NNG_OPT_TCP_KEEPALIVE`

# test
see [`test` directory](test)

## license

MIT

<sub>*currently tested on linux and osx but shortly doing windows support*</sub>
