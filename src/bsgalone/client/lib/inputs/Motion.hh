
#pragma once

#include "Controls.hh"

namespace bsgalone::client {

struct Motion
{
  int x{0};
  int y{0};
  int z{0};

  void updateFromKeys(const pge::controls::State &inputs);
  bool hasMotion() const;
};

} // namespace bsgalone::client
