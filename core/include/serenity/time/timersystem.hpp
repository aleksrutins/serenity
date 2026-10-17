#pragma once
#include <chrono>
#include <cstdint>
#include <atomic>
#include <mutex>
#include <thread>
#include <utility>
#include <functional>
#include <sup.hpp>
#include <serenity/game.hpp>
#include <serenity/entity.hpp>
#include <serenity/world.hpp>

namespace serenity {
namespace time {

class [[sup]] TimerSystem : public Sup {

	std::thread execution_thread;
	std::mutex execution_thread_mutex;

	std::chrono::milliseconds _delta;
	std::atomic_bool exit = false;

	template <int64_t hz = 60>
	void loop(Game *g) {
		using namespace std;
		using namespace std::chrono;

		using frames = duration<int64_t, ratio<1, hz>>;

		auto nextFrame = system_clock::now();
		auto lastFrame = nextFrame - frames{1};

		while(!exit.load()) {
			this_thread::sleep_until(nextFrame);
			_delta = duration_cast<milliseconds>(system_clock::now() - lastFrame);
			{
				std::lock_guard<std::recursive_mutex> worldLock(serenity::worldMutex());
				for(auto e : g->children<Entity>()) e->update(this);
			}
			lastFrame = nextFrame;
			nextFrame += duration_cast<milliseconds>(frames{1});
		}
	}

public:

	TimerSystem(Sup *parent) : Sup(parent) {}

	virtual ~TimerSystem() {
		stop();
	}

	template <int hz = 60>
	void start() {
		stop();
		std::lock_guard<std::mutex> lock(execution_thread_mutex);
		exit.store(false);
		execution_thread = std::thread(&TimerSystem::loop<hz>, this, findParent<Game>());
	}

	void stop() {
		std::thread worker;
		{
			std::lock_guard<std::mutex> lock(execution_thread_mutex);
			requestStop();
			if(execution_thread.joinable() && execution_thread.get_id() != std::this_thread::get_id()) {
				worker = std::move(execution_thread);
			}
		}
		if(worker.joinable()) worker.join();
	}

	void requestStop() {
		exit.store(true);
	}


	auto delta() { return _delta; }
};

}
}
