
#pragma once

#include "TransformedViewFrame.hh"
#include <eigen3/Eigen/Eigen>

namespace pge {

class IsometricViewFrame : public TransformedViewFrame
{
  public:
  IsometricViewFrame(const CenteredViewport &tiles, const TopLeftViewport &pixels);
};

} // namespace pge

#include "IsometricViewFrame.hxx"
