#include <iostream>

#include "player.hpp"
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

    auto sprites = Spritesheet::forScene(&g, TILEMAP_PATH, vec2(32, 32));

    auto p = new Player(&g, sprites);

    cam->follow(p, 0.01);

    auto ground = new Entity(&g);
    sprites->load(ground, vec2(0, 0));
    new Transform(ground, vec2(0, 50));
    new CollisionBox(ground, CollisionBox::DoNotAvoid);

    auto r = new Renderer(&g, "Don't Look Back", WIDTH, HEIGHT);
    new Painter(r);

    g.findChild<TimerSystem>()->start<120>();

    r->run();

    SDL_Quit();

    return 0;
}
