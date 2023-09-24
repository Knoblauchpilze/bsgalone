
#pragma once

#include "TopLeftViewport.hh"

namespace pge {

inline TopLeftViewport::TopLeftViewport(const Vectorf &tl, const Vectorf &dims) noexcept
  : m_topLeft(tl)
  , m_dims(dims)
{}

inline TopLeftViewport::Vectorf TopLeftViewport::center() const noexcept
{
  return Vectorf(m_topLeft.x + m_dims.x / 2.0f, m_topLeft.y + m_dims.y / 2.0f);
}

inline TopLeftViewport::Vectorf TopLeftViewport::topLeft() const noexcept
{
  return m_topLeft;
}

inline TopLeftViewport::Vectorf TopLeftViewport::dims() const noexcept
{
  return m_dims;
}

inline TopLeftViewport::Vectorf TopLeftViewport::relativeCoords(const float x,
                                                                const float y) const noexcept
{
  Vectorf out(x, y);

  out.x -= m_topLeft.x;
  out.y -= m_topLeft.y;

  out.x /= m_dims.x;
  out.y /= m_dims.y;

  return out;
}

inline TopLeftViewport::Vectorf TopLeftViewport::absoluteCoords(const float x,
                                                                const float y) const noexcept
{
  Vectorf out(x, y);

  out.x *= m_dims.x;
  out.y *= m_dims.y;

  out.x += m_topLeft.x;
  out.y += m_topLeft.y;

  return out;
}

inline void TopLeftViewport::moveTo(const Vectorf &topLeft) noexcept
{
  m_topLeft = topLeft;
}

inline void TopLeftViewport::translate(const Vectorf &delta) noexcept
{
  m_topLeft += delta;
}

inline void TopLeftViewport::scale(const float sx, const float sy) noexcept
{
  m_dims.x *= sx;
  m_dims.y *= sy;
}

inline bool TopLeftViewport::visible(const float x,
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
