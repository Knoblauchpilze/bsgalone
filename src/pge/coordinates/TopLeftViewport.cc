
#include "TopLeftViewport.hh"

namespace pge {

TopLeftViewport::TopLeftViewport(const Vec2f &tl, const Vec2f &dims) noexcept
  : m_topLeft(tl)
  , m_dims(dims)
{}

auto TopLeftViewport::center() const noexcept -> Vec2f
{
  return Vec2f(m_topLeft.x + m_dims.x / 2.0f, m_topLeft.y + m_dims.y / 2.0f);
}

auto TopLeftViewport::topLeft() const noexcept -> Vec2f
{
  return m_topLeft;
}

auto TopLeftViewport::dims() const noexcept -> Vec2f
{
  return m_dims;
}

auto TopLeftViewport::relativeCoords(const float x, const float y) const noexcept -> Vec2f
{
  Vec2f out(x, y);

  out.x -= m_topLeft.x;
  out.y -= m_topLeft.y;

  out.x /= m_dims.x;
  out.y /= m_dims.y;

  return out;
}

auto TopLeftViewport::absoluteCoords(const float x, const float y) const noexcept -> Vec2f
{
  Vec2f out(x, y);

  out.x *= m_dims.x;
  out.y *= m_dims.y;

  out.x += m_topLeft.x;
  out.y += m_topLeft.y;

  return out;
}

void TopLeftViewport::moveTo(const Vec2f &topLeft) noexcept
{
  m_topLeft = topLeft;
}

void TopLeftViewport::translate(const Vec2f &delta) noexcept
{
  m_topLeft += delta;
}

void TopLeftViewport::scale(const float sx, const float sy) noexcept
{
  m_dims.x *= sx;
  m_dims.y *= sy;
}

bool TopLeftViewport::visible(const float x,
                              const float y,
                              const float sx,
                              const float sy) const noexcept
{
  if (x + sx < m_topLeft.x || x - sx > m_topLeft.x + m_dims.x)
  {
    return false;
  }

  if (y + sy < m_topLeft.y || y - sy > m_topLeft.y + m_dims.y)
  {
    return false;
  }

  return true;
}

} // namespace pge
