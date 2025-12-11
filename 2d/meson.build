inc = include_directories('include')

deps = [
	dependency('sup'),
	dependency('sdl3'),
	dependency('sdl3-ttf'),
	serenity_dep,
	serenity_graphics_dep
]

serenity_2d = library('serenity-2d', [
	'painter.cpp',
	'transform.cpp',
	'camera.cpp',
	'sprite.cpp',
	'spritesheet.cpp',
	'collisionbox.cpp',
	'platformerphysics.cpp',
], include_directories: inc, dependencies: deps, install: true)
serenity_2d_dep = declare_dependency(link_with: serenity_2d, dependencies: deps, include_directories: inc)

pkg = import('pkgconfig')
pkg.generate(libraries : [serenity_2d],
             subdirs : ['include'],
             version : '1.0',
             name : 'libserenity_2d',
             filebase : 'serenity_2d',
             description : '2D graphics for a game engine for C++ and SDL3')
