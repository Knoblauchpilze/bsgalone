
#pragma once

#include "AxisAlignedBoundingBox.hh"
#include "CoordinateFrame.hh"

namespace pge {

auto toIBoundingBox(const CoordinateFrame &frame) -> bsgo::AxisAlignedBoundingBox;

} // namespace pge
