const nng = require('..')

module.exports  = nngt

function nngt(t){
  t.plan(1)

  nng.test()

  t.ok ( 'nngjs', 'or nodenng')
}
