
{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "sync.cc"
      ],
      "include_dirs": ["<!(node -e \"require('nan')\")"]
    }
  ]
}
