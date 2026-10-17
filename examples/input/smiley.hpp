#pragma once
#include <serenity/core.hpp>
#include <serenity/2d.hpp>
#include <serenity/input.hpp>

using namespace std;
using namespace serenity;
using namespace serenity::math;
using namespace serenity::time;
using namespace serenity::graphics;
using namespace serenity::twod;
using namespace serenity::input;

#define SPEED 5

class Smiley : public Entity {
	SDL_Texture *tex;

	Vec2f go;

	bool quit = false;
public:
	SUP_ISA(Smiley, Entity)

	Smiley(Game *g) : Entity(g, "smiley") {
		auto tr = new Transform(this);

		new OnTick(this, OnTick::none, [this, tr](TimerSystem *) {
			auto k = keyboard::state();

			if(k[SDL_SCANCODE_W]) go[y] -= SPEED;
			if(k[SDL_SCANCODE_A]) go[x] -= SPEED;
			if(k[SDL_SCANCODE_S]) go[y] += SPEED;
			if(k[SDL_SCANCODE_D]) go[x] += SPEED;

			if(k[SDL_SCANCODE_Q]) {
				findParent<Game>()->findChild<Renderer>()->quit();
				return;
			}

			tr->position(lerp(tr->position(), go, 0.025));
	    	});

		Sprite::load(this, SMILEY_PATH, vec2(32.0, 32.0));
	}
};
