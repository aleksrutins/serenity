#define SOL_ALL_SAFETIES_ON 1
#include <serenity/core.hpp>
#include <serenity/graphics.hpp>
#include <serenity/2d.hpp>
#include <sol/sol.hpp>
#include <SDL3/SDL.h>

#include <string>
#include <iostream>
#include <functional>
#include <optional>

using namespace std;
using namespace serenity;

int main(int argc, char **argv) {

    Game app;
	sol::state lua;
	lua.open_libraries(sol::lib::base);

	#include "usertypes.hpp"

	if(argc > 1) {
	    lua.script_file(argv[1]);
    	} else {
    	    stringstream script;
    	    cin.get(*script.rdbuf(), char_traits<char>::eof());

        lua.script(script.view());
    	}

    	SDL_Quit();

	return 0;
}
