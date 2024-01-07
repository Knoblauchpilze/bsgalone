
#include "CenteredViewport.hh"

namespace pge {

CenteredViewport::CenteredViewport(const Vec2f &center, const Vec2f &dims) noexcept
  : Viewport()
  , m_center(center)
  , m_dims(dims)
{}

auto CenteredViewport::center() const noexcept -> Vec2f
{
  return m_center;
}

auto CenteredViewport::topLeft() const noexcept -> Vec2f
{
  return Vec2f(m_center.x - m_dims.x / 2.0f, m_center.y + m_dims.y / 2.0f);
}

auto CenteredViewport::dims() const noexcept -> Vec2f
{
  return m_dims;
}

auto CenteredViewport::relativeCoords(const float x, const float y) const noexcept -> Vec2f
{
  Vec2f out(x, y);

  out.x -= m_center.x;
  out.y -= m_center.y;

  out.x /= (m_dims.x / 2.0f);
  out.y /= (m_dims.y / 2.0f);

  return out;
}

auto CenteredViewport::absoluteCoords(const float x, const float y) const noexcept -> Vec2f
{
  Vec2f out(x, y);

  out.x *= (m_dims.x / 2.0f);
  out.y *= (m_dims.y / 2.0f);

  out.x += m_center.x;
  out.y += m_center.y;

  return out;
}

void CenteredViewport::moveTo(const Vec2f &center) noexcept
{
  m_center = center;
}

void CenteredViewport::translate(const Vec2f &delta) noexcept
{
  m_center += delta;
}

void CenteredViewport::scale(const float sx, const float sy) noexcept
{
  m_dims.x *= sx;
  m_dims.y *= sy;
}

bool CenteredViewport::visible(const float x,
                               const float y,
                               const float sx,
                               const float sy) const noexcept
{
  if (x + sx < m_center.x - m_dims.x / 2.0f || x - sx > m_center.x + m_dims.x / 2.0f)
  {
    return false;
  }

  if (y + sy < m_center.y - m_dims.y / 2.0f || y - sy > m_center.y + m_dims.y / 2.0f)
  {
    return false;
  }

  return true;
}

} // namespace pge
