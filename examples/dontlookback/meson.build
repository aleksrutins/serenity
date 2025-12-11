data_dir = get_option('datadir') / 'serenity' / 'examples'
install_data('tilemap.png', install_dir : data_dir)
tilemap_path = get_option('prefix') / data_dir / 'tilemap.png'
executable('serenity-example-dontlookback', 'main.cpp', dependencies : [serenity_dep, serenity_graphics_dep, serenity_2d_dep, serenity_input_dep], install: true, cpp_args: '-DTILEMAP_PATH="' + tilemap_path + '"')
