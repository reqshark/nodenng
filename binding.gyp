{'targets': [
    {
        'target_name': 'nodenng',
        'conditions': [
#            ['OS=="linux"', {
#                'cflags': [ '-fPIC' ],
#            }],
        ],
        'libraries': [ '../deps/lib/libnng.dylib' ],
        'include_dirs': [
            "<!(node -e \"require('nan')\")",
            "deps/include"
        ],
        'sources': [ 'binding.cc' ],
    }
]}
