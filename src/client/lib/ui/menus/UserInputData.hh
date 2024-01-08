
#pragma once

#include "Controls.hh"
#include <functional>

namespace pge {

class Game;

using GameCallback = std::function<void(Game &)>;

struct UserInputData
{
  controls::State controls{};
  std::vector<GameCallback> actions{};
};

} // namespace pge
