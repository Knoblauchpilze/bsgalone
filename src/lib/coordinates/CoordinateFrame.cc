
#include "CoordinateFrame.hh"

namespace pge {

CoordinateFrame::CoordinateFrame(const CenteredViewport &tiles, const TopLeftViewport &pixels)
  : utils::CoreObject("frame")
  , m_tilesViewport(tiles)
  , m_pixelsViewport(pixels)
{
  setService("coordinate");
}

olc::vf2d CoordinateFrame::tileSize() const noexcept
{
  const auto tilesDims  = m_tilesViewport.dims();
  const auto pixelsDims = m_pixelsViewport.dims();

  return pixelsDims / tilesDims;
}

CenteredViewport CoordinateFrame::tilesViewport() const noexcept
{
  return m_tilesViewport;
}

olc::vf2d CoordinateFrame::tilesToPixels(float x, float y) const noexcept
{
  auto rel = m_tilesViewport.relativeCoords(x, y);

  auto transformed = normalizedTilesToPixels(rel);

  // Tiles viewport is centered, pixels viewport is top left based.
  // The minimum coordinate for the centered is `-1` and so we need
  // to offset this. Also as it ranges from `[-1; 1]`, we need to
  // half the value as the pixels only ranges from `[0; 1]`.
  transformed.x = (transformed.x + 1.0f) / 2.0f;
  // Tiles viewport is with y up, pixels viewport is with y down so
  // the 1 of the tiles is the 0 of the pixels and so we need to map
  // negatively `y`. Similarly as for `x`, tiles are ranging from
  // `[-1; 1]` while pixels go from `[0; 1]` so we need to half the
  // value.
  transformed.y = (1.0f - transformed.y) / 2.0f;

  return m_pixelsViewport.absoluteCoords(transformed.x, transformed.y);
}

olc::vf2d CoordinateFrame::pixelsToTiles(float x, float y) const noexcept
{
  auto rel = m_pixelsViewport.relativeCoords(x, y);
  // Reverse operation of the `tilesToPixels`.
  rel.x = 2.0f * rel.x - 1.0f;
  rel.y = 1.0f - 2.0f * rel.y;

  auto transformed = normalizedPixelsToTiles(rel);

  return m_tilesViewport.absoluteCoords(transformed.x, transformed.y);
}

olc::vi2d CoordinateFrame::pixelsToTilesAndIntra(const olc::vf2d &pixels,
                                                 olc::vf2d *intraTile) const noexcept
{
  auto tiles = pixelsToTiles(pixels.x, pixels.y);

  olc::vi2d out(static_cast<int>(std::floor(tiles.x)), static_cast<int>(std::floor(tiles.y)));

  if (intraTile != nullptr)
  {
    intraTile->x = tiles.x - out.x;
    intraTile->y = tiles.y - out.y;
  }

  return out;
}

void CoordinateFrame::zoomIn(const olc::vf2d &pos)
{
  zoom(2.0f, pos);
}

void CoordinateFrame::zoomOut(const olc::vf2d &pos)
{
  zoom(0.5f, pos);
}

void CoordinateFrame::beginTranslation(const olc::vf2d &pixelsOrigin)
{
  m_pixelsTranslationOrigin = pixelsOrigin;
  m_tilesCachedPOrigin      = m_tilesViewport.center();
}

void CoordinateFrame::translate(const olc::vf2d &pixelsOrigin)
{
  olc::vf2d originTiles = pixelsToTiles(m_pixelsTranslationOrigin.x, m_pixelsTranslationOrigin.y);
  olc::vf2d posTiles    = pixelsToTiles(pixelsOrigin.x, pixelsOrigin.y);
  olc::vf2d translationTiles = originTiles - posTiles;

  m_tilesViewport.moveTo(m_tilesCachedPOrigin + translationTiles);
}

void CoordinateFrame::zoom(float factor, const olc::vf2d &pos)
{
  olc::vf2d dPixels = pos - m_pixelsViewport.topLeft();

  olc::vf2d pTiles = pixelsToTiles(pos.x, pos.y);
  olc::vf2d dTiles = pTiles - m_tilesViewport.center();

  dPixels /= factor;
  dTiles /= factor;

  m_tilesViewport.moveTo(pTiles - dTiles);

  // Only the dimensions of the tiles viewport need to be updated.
  m_tilesViewport.scale(1.0f / factor, 1.0f / factor);
}

} // namespace pge
