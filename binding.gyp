
{
  "targets": [
    {
      "target_name": "nodenng",
      "sources": [
        "nodenng.cc"
      ],
      "include_dirs": ["<!(node -e \"require('nan')\")"]
    }
  ]
}
