
#include "TransformedViewFrame.hh"

/// Links:
/// http://archive.gamedev.net/archive/reference/programming/features/mm4ihm/index.html
/// https://gamedev.stackexchange.com/questions/142784/how-do-you-transform-a-coordinate-system-from-cartesian-to-isometric
/// https://gamedev.stackexchange.com/questions/31113/how-can-i-improve-my-isometric-tile-picking-algorithm/31124#31124
/// https://stackoverflow.com/questions/21842814/mouse-position-to-isometric-tile-including-height

/// Promising:
/// https://pikuma.com/blog/isometric-projection-in-games

namespace pge {

TransformedViewFrame::TransformedViewFrame(const CenteredViewport &tiles,
                                           const Eigen::Matrix2f &transform,
                                           const TopLeftViewport &pixels)
  : CoordinateFrame(tiles, pixels)
{
  m_tilesToPixelsTransform = transform;
  m_pixelsToTilesTransform = m_tilesToPixelsTransform.inverse();
}

olc::vf2d TransformedViewFrame::normalizedTilesToPixels(const olc::vf2d &tiles) const noexcept
{
  Eigen::Vector2f transformed = m_tilesToPixelsTransform * Eigen::Vector2f{tiles.x, tiles.y};
  return olc::vf2d{transformed(0), transformed(1)};
}

olc::vf2d TransformedViewFrame::normalizedPixelsToTiles(const olc::vf2d &pixels) const noexcept
{
  Eigen::Vector2f transformed = m_pixelsToTilesTransform * Eigen::Vector2f{pixels.x, pixels.y};
  return olc::vf2d{transformed(0), transformed(1)};
}

} // namespace pge
