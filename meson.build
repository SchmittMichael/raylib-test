project(
  'raylib-test',
  'c',
  version : '0.1.0',
  default_options : [
    'warning_level=3',
    'c_std=c17',
    'werror=false'
  ]
)

inc = include_directories('include')

raylib_dep = dependency('raylib', fallback : ['raylib', 'raylib_dep'])

exe = executable(
  'raylib-test',
  'src/main.c',
  include_directories : inc,
  dependencies: [raylib_dep],
)
