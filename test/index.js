const bar = '===================='
require ('tape')(`${bar} nng testsuite summary ${bar}`, function tests (t){
  test('open')

  function test(name){
    return t.test(`${bar.slice(7)} ${name} ${bar.slice(7)}`,
      require(`./${name}`))
  }
})
