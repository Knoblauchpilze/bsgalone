
#pragma once

#include "TransformedViewFrame.hh"

namespace pge {

class TopViewFrame : public TransformedViewFrame
{
  public:
  TopViewFrame(const CenteredViewport &tiles, const TopLeftViewport &pixels);
};

} // namespace pge
