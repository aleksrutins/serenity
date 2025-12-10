data_dir = get_option('datadir') / 'serenity' / 'examples'
install_data('lettuce.bmp', install_dir : data_dir)
lettuce_path = get_option('prefix') / data_dir / 'lettuce.bmp'
executable('serenity-example-lettuce', 'main.cpp', dependencies : [serenity_dep, serenity_graphics_dep, serenity_2d_dep], install: true, cpp_args: '-DLETTUCE_PATH="' + lettuce_path + '"')
