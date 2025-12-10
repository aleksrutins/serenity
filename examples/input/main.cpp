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

    new Smiley(&g);

    auto r = new Renderer(&g, "Serenity Input Example");
    new Painter(r);

    g.findChild<TimerSystem>()->start<60>();

    r->run();

    return 0;
}
