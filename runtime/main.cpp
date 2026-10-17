#define SOL_ALL_SAFETIES_ON 1
#include <serenity/core.hpp>
#include <serenity/input.hpp>
#include <serenity/graphics.hpp>
#include <serenity/2d.hpp>
#include "lua_types.hpp"
#include <sol/sol.hpp>
#include <SDL3/SDL.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <cstdint>
#include <tuple>

#include <string>
#include <iostream>
#include <functional>
#include <optional>

using namespace std;
using namespace serenity;

int main(int argc, char **argv) {
	sol::state lua;
	{
		using sol::lib;
		lua.open_libraries(
			lib::base,
			lib::package,
			lib::math,
			lib::bit32,
			lib::coroutine,
			lib::count,
			lib::debug,
			lib::ffi,
			lib::io,
			lib::jit,
			lib::os,
			lib::string,
			lib::table,
			lib::utf8);
	}

	{
		Game app;
		#include "usertypes.hpp"

		if(argc > 1) {
			lua.script_file(argv[1]);
		} else {
			stringstream script;
			cin.get(*script.rdbuf(), char_traits<char>::eof());
			lua.script(script.view());
		}
	}

	SDL_Quit();
	return 0;
}
