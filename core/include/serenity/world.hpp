#pragma once

#include <mutex>

namespace serenity {

inline auto worldMutex() -> std::recursive_mutex & {
	static std::recursive_mutex mutex;
	return mutex;
}

}
