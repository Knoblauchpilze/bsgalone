
#pragma once

#include "olcPixelGameEngine.h"
#include <memory>

namespace pge {

struct DecalDeleter
{
  void operator()(olc::Decal *decal) const noexcept;
};

using DecalPtr = std::unique_ptr<olc::Decal, DecalDeleter>;

} // namespace pge
