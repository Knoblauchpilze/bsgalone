
#pragma once

#include "Action.hh"
#include "Controls.hh"

namespace pge {

struct UserInputData
{
  controls::State controls{};
  std::vector<ActionPtr> actions{};
};

} // namespace pge
