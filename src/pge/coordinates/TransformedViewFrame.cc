
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

auto TransformedViewFrame::normalizedTilesToPixels(const Vec2f &tiles) const noexcept -> Vec2f
{
  Eigen::Vector2f transformed = m_tilesToPixelsTransform * Eigen::Vector2f{tiles.x, tiles.y};
  return Vec2f{transformed(0), transformed(1)};
}

auto TransformedViewFrame::normalizedPixelsToTiles(const Vec2f &pixels) const noexcept -> Vec2f
{
  Eigen::Vector2f transformed = m_pixelsToTilesTransform * Eigen::Vector2f{pixels.x, pixels.y};
  return Vec2f{transformed(0), transformed(1)};
}

} // namespace pge
