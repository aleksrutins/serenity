inc = include_directories('include')
serenity2d = library('serenity2d', [
#	'camera.cpp',
	#'cameraentity.cpp',

], include_directories: inc, dependencies: [dependency('sup'), dependency('sdl3'), serenity_dep], install: true)
serenity2d_dep = declare_dependency(link_with: serenity, dependencies: [dependency('sup'), dependency('sdl3'), serenity_dep], include_directories: inc)

pkg = import('pkgconfig')
pkg.generate(libraries : [serenity2d],
             subdirs : ['include'],
             version : '1.0',
             name : 'libserenity2d',
             filebase : 'serenity2d',
             description : 'A game engine for C++ and SDL3')
