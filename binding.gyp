{'targets': [
    {
        'target_name': 'nodenng',
        'conditions': [
            ['OS=="linux"', {
                'cflags': [ '-fPIC' ],
            }],
#            ['OS=="mac"', {
#                'xcode_settings': {
#                    'OTHER_CFLAGS': [ '-std=c++11' ],
#                },
#            }],
        ],

        'libraries': [ '../deps/lib/libnng.a' ],
        'include_dirs': [
            "<!(node -e \"require('nan')\")",
            "deps/include"
        ],
        'sources': [ 'binding.cc' ],
    }
]}
