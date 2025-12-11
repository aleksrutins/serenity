data_dir = get_option('datadir') / 'serenity' / 'examples'
install_data('smiley.bmp', install_dir : data_dir)
smiley_path = get_option('prefix') / data_dir / 'smiley.bmp'
executable('serenity-example-input', 'main.cpp', dependencies : [serenity_dep, serenity_graphics_dep, serenity_2d_dep, serenity_input_dep], install: true, cpp_args: ['-fext-numeric-literals', '-DSMILEY_PATH="' + smiley_path + '"'])
