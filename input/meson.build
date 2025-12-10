inc = include_directories('include')

deps = [
	dependency('sup'),
	dependency('sdl3'),
	serenity_dep,
	serenity_graphics_dep
]

serenity_input = library('serenity-input', [
	'keyboard.cpp',
], include_directories: inc, dependencies: deps, install: true)
serenity_input_dep = declare_dependency(link_with: serenity_input, dependencies: deps, include_directories: inc)

pkg = import('pkgconfig')
pkg.generate(libraries : [serenity_input],
             subdirs : ['include'],
             version : '1.0',
             name : 'libserenity_input',
             filebase : 'serenity_input',
             description : 'Input handline for a game engine for C++ and SDL3')
