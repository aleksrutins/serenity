#pragma once
#include <chrono>
#include <cstdint>
#include <thread>
#include <functional>
#include <sup.hpp>
#include <serenity/game.hpp>
#include <serenity/entity.hpp>

namespace serenity {
namespace time {

class TimerSystem : public Sup {

	std::thread execution_thread;

	std::chrono::milliseconds _delta;
	bool exit = false;

	template <int64_t hz = 60>
	void loop(Game *g) {
		using namespace std;
		using namespace std::chrono;

		using frames = duration<int64_t, ratio<1, hz>>;

		auto nextFrame = system_clock::now();
		auto lastFrame = nextFrame - frames{1};

		while(!exit) {
			this_thread::sleep_until(nextFrame);
			_delta = duration_cast<milliseconds>(system_clock::now() - lastFrame);
			for(auto e : g->children<Entity>()) e->update(this);
			lastFrame = nextFrame;
			nextFrame += duration_cast<milliseconds>(frames{1});
		}
	}

public:
	TimerSystem(Sup *parent) : Sup(parent) {}

	virtual ~TimerSystem() {
		if(!exit) stop();
	}

	template <int hz = 60>
	void start() {
		execution_thread = std::thread(&TimerSystem::loop<hz>, this, findParent<Game>());
	}

	void stop() {
		exit = true;
		execution_thread.detach();
	}


	auto delta() { return _delta; }
};

}
}