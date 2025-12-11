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

#define WIDTH 800
#define HEIGHT 600

int main() {
    Game g;

    new TimerSystem(&g);

    auto cam = new Camera(&g, vec2(WIDTH, HEIGHT));

    auto s = new Smiley(&g);

    cam->follow(s, 0.01);

    auto r = new Renderer(&g, "Serenity Camera Example", WIDTH, HEIGHT);
    new Painter(r);

    g.findChild<TimerSystem>()->start<120>();

    r->run();

    SDL_Quit();

    return 0;
}
