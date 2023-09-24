
#pragma once

#include "CenteredViewport.hh"

namespace pge {

inline CenteredViewport::CenteredViewport(const Vectorf &center, const Vectorf &dims) noexcept
  : Viewport()
  , m_center(center)
  , m_dims(dims)
{}

inline CenteredViewport::Vectorf CenteredViewport::center() const noexcept
{
  return m_center;
}

inline CenteredViewport::Vectorf CenteredViewport::topLeft() const noexcept
{
  return Vectorf(m_center.x - m_dims.x / 2.0f, m_center.y + m_dims.y / 2.0f);
}

inline CenteredViewport::Vectorf CenteredViewport::dims() const noexcept
{
  return m_dims;
}

inline CenteredViewport::Vectorf CenteredViewport::relativeCoords(const float x,
                                                                  const float y) const noexcept
{
  Vectorf out(x, y);

  out.x -= m_center.x;
  out.y -= m_center.y;

  out.x /= (m_dims.x / 2.0f);
  out.y /= (m_dims.y / 2.0f);

  return out;
}

inline CenteredViewport::Vectorf CenteredViewport::absoluteCoords(const float x,
                                                                  const float y) const noexcept
{
  Vectorf out(x, y);

  out.x *= (m_dims.x / 2.0f);
  out.y *= (m_dims.y / 2.0f);

  out.x += m_center.x;
  out.y += m_center.y;

  return out;
}

inline void CenteredViewport::moveTo(const Vectorf &center) noexcept
{
  m_center = center;
}

inline void CenteredViewport::translate(const Vectorf &delta) noexcept
{
  m_center += delta;
}

inline void CenteredViewport::scale(const float sx, const float sy) noexcept
{
  m_dims.x *= sx;
  m_dims.y *= sy;
}

inline bool CenteredViewport::visible(const float x,
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
