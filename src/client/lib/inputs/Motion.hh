
#pragma once

#include "Controls.hh"

namespace pge {

struct Motion
{
  int x{0};
  int y{0};
  int z{0};

  void updateFromKeys(const controls::State &inputs);
};

} // namespace pge
