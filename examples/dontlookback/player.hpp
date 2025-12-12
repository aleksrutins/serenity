#pragma once

#include "collectible.hpp"
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

class Player : public Entity {
	friend class Collectible;
	float speed = 4;
public:
	Player(Game *g, Spritesheet *sprites) : Entity(g, "smiley") {
		sprites->load(this, vec2(0, 5));
		auto tr = new Transform(this);
		auto box = new CollisionBox(this, CollisionBox::DoNotAvoid);
		auto ph = new PlatformerPhysics(this);

		new OnTick(this, OnTick::none, [this, tr, box, ph](TimerSystem *) {
			auto k = keyboard::state();

			if(k[SDL_SCANCODE_W] && box->hasCollision()) ph->jump();
			if(k[SDL_SCANCODE_A]) tr->translate(-speed, 0);
			if(k[SDL_SCANCODE_S]) {
				tr->translate(0, speed);
				tr->delta = vec2(0, 0);
			}
			if(k[SDL_SCANCODE_D]) tr->translate(speed, 0);

			if(k[SDL_SCANCODE_Q]) findParent<Game>()->findChild<Renderer>()->quit();

			auto collectibles = findParent<Game>()->children<Collectible>(true);
			for (auto c : collectibles) {
				auto cbox = c->findChild<CollisionBox>();
				if(box->rect().isZero() || cbox->rect().isZero()) return;
				if(box->overlaps(cbox)) {
					speed += .2;
					ph->jumpSpeed += .2;
					cout << "[collectible] " << speed << endl;
					findParent<Game>()->findChild<Camera>()->follow(this, 0.005 * speed);
					c->parent()->remove(c);
				}
			}

	    	});
	}
};