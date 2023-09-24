
#pragma once

#include "TopViewFrame.hh"

namespace pge {

inline TopViewFrame::TopViewFrame(const CenteredViewport &tiles, const TopLeftViewport &pixels)
  : // No transformation between the orientation of the pixels space and the tiles space.
  TransformedViewFrame(tiles, Eigen::Matrix2f::Identity(), pixels)
{}

} // namespace pge
