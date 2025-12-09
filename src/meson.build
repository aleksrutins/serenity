inc = include_directories('include')
serenity = library('serenity', [
	'scene.cpp',
	'game.cpp',
	'entity.cpp',
	'component.cpp',
	'system.cpp',
	'time/ontick.cpp'
], include_directories: inc, dependencies: [dependency('sup')], install: true)
serenity_dep = declare_dependency(link_with: serenity, dependencies: [dependency('sup')], include_directories: inc)

pkg = import('pkgconfig')
pkg.generate(libraries : [serenity],
             subdirs : ['include'],
             version : '1.0',
             name : 'libserenity',
             filebase : 'serenity',
             description : 'A game engine for C++ and SDL3')
