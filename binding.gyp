{
  'targets': [
    {
      'target_name': 'node_dablooms',
      'sources': [
        'src/node_dablooms.cc',
        'src/dablooms.cc'
      ],
      'conditions': [
        ['OS!="win"', {
          'link_settings': {
            'libraries': [
              '-ldablooms',
            ],
          },
          'include_dirs': [
            '/usr/local/include'
          ],
          'ldflags': [
            '-L /usr/local/lib'
          ]
        }]
      ]
    }
  ]
}
