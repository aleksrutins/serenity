#pragma once

#include <iostream>

#include <serenity/core.hpp>
#include <serenity/2d.hpp>

using namespace std;
using namespace serenity;
using namespace serenity::math;
using namespace serenity::time;
using namespace serenity::twod;

class Tile : public Entity {
public:
	Tile(Scene *s, Spritesheet *sprites, Vec2f position) : Entity(s) {
		sprites->load(this, vec2(0, 0));
		new Transform(this, position);
		new CollisionBox(this, CollisionBox::DoNotAvoid, vec2(32, 32));
	}
};