# nodenng

bindings to nng (nanomsg next generation)

# install
```bash
npm i nodenng
```

# use
```js
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
} = require('nodenng')


const pub = pub_open()
const sub = sub_open()

// more nng api will be made available to javascript in the coming days
// please help add stuff or send PRs if you'd like faster access from node.js
```

# test
see [`test` directory](test)

## license

MIT. &copy; 2019 Bent Cardan

<sub>*currently tested on linux and osx but very shortly full windows support*</sub>
