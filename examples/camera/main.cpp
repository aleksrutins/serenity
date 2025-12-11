#include <iostream>

#include "smiley.hpp"
#include <serenity/core.hpp>
#include <serenity/graphics.hpp>
#include <serenity/2d.hpp>
#include <serenity/input.hpp>

using namespace std;
using namespace std::chrono;
using namespace serenity;
using namespace serenity::time;
using namespace serenity::graphics;
using namespace serenity::twod;
using namespace serenity::input;

int main() {
    Game g;

    new TimerSystem(&g);

    auto cam = new Camera(&g);

    auto s = new Smiley(&g);

    //cam->follow(s, 0.001);

    auto r = new Renderer(&g, "Serenity Camera Example");
    new Painter(r);

    g.findChild<TimerSystem>()->start<120>();

    r->run();

    SDL_Quit();

    return 0;
}
