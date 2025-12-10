#pragma once
#include <iostream>
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

	double actX = 0;
	double actY = 0;
	double x = 0;
	double y = 0;
	int w = 32;
	int h = 32;

	bool quit = false;

	inline auto rect(Painter *p) {return p->rectangle(actX, actY, w, h);}
public:
	Smiley(Game *g) : Entity(g, "smiley") {
		new OnTick(this, OnTick::none, [this](TimerSystem *) {
			auto k = keyboard::state();

			if(k[SDL_SCANCODE_W]) y -= SPEED;
			if(k[SDL_SCANCODE_A]) x -= SPEED;
			if(k[SDL_SCANCODE_S]) y += SPEED;
			if(k[SDL_SCANCODE_D]) x += SPEED;

			if(k[SDL_SCANCODE_Q]) quit = true;

			actX = lerp(actX, x, 0.025);
	    		actY = lerp(actY, y, 0.025);
	    	});

		new CustomRender(this, [this](Renderer *r) {
			cout << "init" << endl;
			auto p = r->findChild<Painter>();
			tex = p->loadImage(SMILEY_PATH);
	    	}, [this](Renderer *r) {
	    		auto p = r->findChild<Painter>();
	    		auto target = rect(p);
			p->draw(tex, &target);

			if(quit) r->quit();
	    	});

	    	findChild<OnTick>()->update(g->findChild<TimerSystem>());
	}
};