#pragma once
#include <serenity/component.hpp>
#include <serenity/passthrough.hpp>
#include <serenity/time/timersystem.hpp>
#include <functional>

namespace serenity {
namespace time {

using OnTick = serenity::Passthrough<TimerSystem>;

}
}