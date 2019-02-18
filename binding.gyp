{'targets': [
    {
        'target_name': 'nodenng',
        'libraries': [ '../nng/build/libnng.a' ],
        'include_dirs': [
            "<!(node -e \"require('nan')\")",
        ],
        'sources': [ 'binding.cc' ],
    }
]}
