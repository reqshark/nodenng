const bar = '===================='
require ('tape')(`${bar} nng testsuite summary ${bar}`, function tests (t){

  /* use single continguous characters of the same filenames */
  test('open')
  test('listen')


  function test(name){
    return t.test(`${bar.slice(7)} ${name} ${bar.slice(7)}`,
      require(`./${name}`))
  }
})
