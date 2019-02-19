{'targets': [
    {
        'target_name': 'nodenng',
        'libraries': [ '../deps/lib/libnng.a' ],
        'include_dirs': [
            "<!(node -e \"require('nan')\")",
            "deps/include"
        ],
        'sources': [ 'binding.cc' ],
    }
]}
