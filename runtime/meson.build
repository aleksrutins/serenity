executable(
	'serenity',
	'main.cpp',
	dependencies : [
		serenity_dep,
		serenity_graphics_dep,
		serenity_2d_dep,
		serenity_input_dep,
		dependency('lua')
	],
	install: true
)
install_data('runtime_test.lua', rename : 'serenity-runtime-test', install_dir : 'bin', install_mode : 'rwxr-xr-x')