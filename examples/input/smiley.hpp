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

	Vec2<float> go;
	Vec2<float> &pos() {return findChild<Transform>()->position;};

	bool quit = false;
public:
	Smiley(Game *g) : Entity(g, "smiley") {
		new Transform(this);

		new OnTick(this, OnTick::none, [this](TimerSystem *) {
			auto k = keyboard::state();

			if(k[SDL_SCANCODE_W]) go[y] -= SPEED;
			if(k[SDL_SCANCODE_A]) go[x] -= SPEED;
			if(k[SDL_SCANCODE_S]) go[y] += SPEED;
			if(k[SDL_SCANCODE_D]) go[x] += SPEED;

			if(k[SDL_SCANCODE_Q]) {
				findParent<Game>()->findChild<Renderer>()->quit();
				return;
			}

			pos() = lerp(pos(), go, 0.025);
	    	});

		new Sprite(this, SMILEY_PATH, vec2(32, 32));
	}
};