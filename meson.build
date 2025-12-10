project(
  'raylib-test',
  'c',
  version : '0.1.0',
  default_options : [
    'warning_level=3',
    'c_std=c17',
    'werror=false',
  ]
)

src_files = ['src/main.c', 'src/ball_demo.c', 'src/pong_demo.c']

raylib_dep = dependency('raylib', fallback : ['raylib', 'raylib_dep'])

cc = meson.get_compiler('c')
m_dep = cc.find_library('m')

exe = executable(
  'raylib-test',
  src_files,
  dependencies: [raylib_dep, m_dep],
)
