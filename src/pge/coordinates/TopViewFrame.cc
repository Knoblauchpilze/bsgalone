
#include "TopViewFrame.hh"

namespace pge {

// No transformation between the orientation of the pixels space and the tiles space.
TopViewFrame::TopViewFrame(const CenteredViewport &tiles, const TopLeftViewport &pixels)
  : TransformedViewFrame(tiles, Eigen::Matrix2f::Identity(), pixels)
{}

} // namespace pge
