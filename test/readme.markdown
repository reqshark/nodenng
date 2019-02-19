# build and test [![CircleCI](https://circleci.com/gh/reqshark/nodenng/tree/master.svg?style=svg)](https://circleci.com/gh/reqshark/nodenng/tree/master)

`test/index.js` file runs the tests by iterating contents top-down within this
directory.

process is launched from project root by `node test` piped to `tap-spec`.
```js
$ node test | tap-spec
```

there are also some convenience operations via make:
```bash
$ git clone git@github.com:reqshark/nodenng.git && cd nodenng
$ make clean && make && make check
```

these are respectively:
```bash
$ rm -rf package-* node_modules build nng deps # make clean
$ npm i                                        # make
$ npm t                                        # make check
```
