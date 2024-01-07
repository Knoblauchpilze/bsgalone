
#pragma once

#include "CoordinateFrame.hh"
#include <eigen3/Eigen/Eigen>

namespace pge {

class TransformedViewFrame : public CoordinateFrame
{
  public:
  TransformedViewFrame(const CenteredViewport &tiles,
                       const Eigen::Matrix2f &transform,
                       const TopLeftViewport &pixels);

  protected:
  auto normalizedTilesToPixels(const olc::vf2d &tiles) const noexcept -> olc::vf2d override;
  auto normalizedPixelsToTiles(const olc::vf2d &pixels) const noexcept -> olc::vf2d override;

  private:
  /// @brief - The transformation matrix to convert from tiles space to pixels
  /// space. This doesn't take into consideration scaling and is meant to work
  /// in normalized coordinate space.
  Eigen::Matrix2f m_tilesToPixelsTransform;

  /// @brief - The inverse matrix from the previous one.
  Eigen::Matrix2f m_pixelsToTilesTransform;
};

} // namespace pge
