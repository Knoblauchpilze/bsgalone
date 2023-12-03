
#pragma once

#include "Controls.hh"

namespace pge {

class Game;

using GameCallback = std::function<void(Game &)>;

struct UserInputData
{
  controls::State controls{};
  std::vector<GameCallback> actions{};
};

} // namespace pge
