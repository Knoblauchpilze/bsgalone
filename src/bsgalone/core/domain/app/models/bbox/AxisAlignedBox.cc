
#include "AxisAlignedBox.hh"

namespace bsgalone::core {

AxisAlignedBox::AxisAlignedBox(const Eigen::Vector3f &center, const Eigen::Vector3f &dims) noexcept
  : m_center(center)
  , m_dims(dims)
{}

auto AxisAlignedBox::dims() const -> Eigen::Vector3f
{
  return m_dims;
}

auto AxisAlignedBox::position() const -> Eigen::Vector3f
{
  return m_center;
}

void AxisAlignedBox::moveTo(const Eigen::Vector3f &position)
{
  m_center = position;
}

bool AxisAlignedBox::isInside(const Eigen::Vector3f &pos) const noexcept
{
  Eigen::Vector3f frontBottomLeft = m_center - m_dims / 2.0f;
  Eigen::Vector3f backTopRight    = m_center + m_dims / 2.0f;

  if (pos(0) < frontBottomLeft(0))
  {
    return false;
  }
  if (pos(0) > backTopRight(0))
  {
    return false;
  }

  if (pos(1) < frontBottomLeft(1))
  {
    return false;
  }
  if (pos(1) > backTopRight(1))
  {
    return false;
  }

  if (pos(2) < frontBottomLeft(2))
  {
    return false;
  }
  if (pos(2) > backTopRight(2))
  {
    return false;
  }

  return true;
}

void AxisAlignedBox::translate(const Eigen::Vector3f &delta)
{
  m_center += delta;
}

} // namespace bsgalone::core
