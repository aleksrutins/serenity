#pragma once
#include <serenity/core.hpp>
#include <serenity/2d.hpp>
#include <serenity/input.hpp>
#include <iostream>

using namespace std;
using namespace serenity;
using namespace serenity::math;
using namespace serenity::time;
using namespace serenity::graphics;
using namespace serenity::twod;
using namespace serenity::input;

#define SPEED 5

class Smiley : public Entity {
	float &x() {return findChild<Transform>()->position[math::x];};
	float &y() {return findChild<Transform>()->position[math::y];};
public:
	Smiley(Game *g) : Entity(g, "smiley") {
		new Transform(this);
		new OnTick(this, OnTick::none, [this](TimerSystem *) {
			auto k = keyboard::state();

			if(k[SDL_SCANCODE_W]) y() -= SPEED;
			if(k[SDL_SCANCODE_A]) x() -= SPEED;
			if(k[SDL_SCANCODE_S]) y() += SPEED;
			if(k[SDL_SCANCODE_D]) x() += SPEED;

			if(k[SDL_SCANCODE_Q]) findParent<Game>()->findChild<Renderer>()->quit();
	    	});

	    	new Sprite(this, SMILEY_PATH, vec2(32, 32));
	}
};