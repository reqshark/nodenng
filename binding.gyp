{'targets': [
    {
        'target_name': 'nodenng',
        'libraries': [ '../nngclone/build/libnng.a' ],
        'include_dirs': [
            "<!(node -e \"require('nan')\")",
        ],
        'sources': [ 'binding.cc' ],
    }
]}
