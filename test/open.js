const nng = require('..')

module.exports  = open

function open(t){
  t.plan(11)

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
  } = nng

  t.ok ( Buffer.isBuffer(bus_open()), 'open bus sock is a node buffer')
  t.ok ( Buffer.isBuffer(pair0_open()), 'open pair0 sock is a node buffer')
  t.ok ( Buffer.isBuffer(pair1_open()), 'open pair1 sock is a node buffer')
  t.ok ( Buffer.isBuffer(pub_open()), 'open pub sock is a node buffer')
  t.ok ( Buffer.isBuffer(pull_open()), 'open pull sock is a node buffer')
  t.ok ( Buffer.isBuffer(push_open()), 'open push sock is a node buffer')
  t.ok ( Buffer.isBuffer(rep_open()), 'open rep sock is a node buffer')
  t.ok ( Buffer.isBuffer(req_open()), 'open req sock is a node buffer')
  t.ok ( Buffer.isBuffer(respondent_open()), 'open respondent sock is a node buffer')
  t.ok ( Buffer.isBuffer(sub_open()), 'open sub sock is a node buffer')
  t.ok ( Buffer.isBuffer(surveyor_open()), 'open surveyor sock is a node buffer')

}
