#pragma once

#include "constants.hpp"
#include "collectible.hpp"
#include "tile.hpp"

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <serenity/core.hpp>
#include <serenity/2d.hpp>

using namespace std;
using namespace serenity;
using namespace serenity::time;
using namespace serenity::twod;
using namespace serenity::math;

class Generator : public Entity {
	float generatedTo = 0;
public:
	Generator(Scene *s, Spritesheet *sprites, Camera *cam) : Entity(s) {
		srand((unsigned)std::time(0));
		new OnTick(this, OnTick::none, [this, s, sprites, cam](TimerSystem*) {
			long distance = ceilf(((cam->findChild<Transform>()->position()[x] + WIDTH) - (generatedTo + 32) + 64) / 32) * 32;
			if(distance) {
				for(long i = 0; i < distance; i += 32) {
					for(int j = 0; j < HEIGHT; j += 32) {
						auto randn = rand() % 100;
						if(randn < 4) {
							new Tile(s, sprites, vec2<float>(generatedTo + i, j));
						} else if(randn < 6) {
							new Collectible(s, sprites, vec2<float>(generatedTo + i, j));
						}
					}
				}
			}
			generatedTo += distance;
		});
	}
};