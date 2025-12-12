project('serenity', ['cpp', 'c'],
          version: '0.1.0',
    meson_version: '>= 1.0.0',
  default_options: [ 'warning_level=2', 'werror=false', 'cpp_std=c++2a' ],
)

add_project_arguments('-Ofast', language: 'cpp')

subdir('core')
subdir('graphics')
subdir('2d')
subdir('input')

subdir('runtime')

subdir('examples')
subdir('example-launcher')