
#pragma once

#include "AxisAlignedBoundingBox.hh"
#include "CoordinateFrame.hh"

namespace bsgalone::client {

auto toIBoundingBox(const pge::CoordinateFrame &frame) -> core::AxisAlignedBoundingBox;

} // namespace bsgalone::client
