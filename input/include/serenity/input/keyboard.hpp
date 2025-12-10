#pragma once
#include <SDL3/SDL_keyboard.h>

namespace serenity {
namespace input {
namespace keyboard {

inline auto state() { return SDL_GetKeyboardState(NULL); }

}
}
}