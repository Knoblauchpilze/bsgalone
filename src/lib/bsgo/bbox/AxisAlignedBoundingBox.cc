
#include "AxisAlignedBoundingBox.hh"

namespace bsgo {

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Eigen::Vector3f &center,
                                               const Eigen::Vector3f &dims) noexcept
  : m_center(center)
  , m_dims(dims)
{}

auto AxisAlignedBoundingBox::position() const -> Eigen::Vector3f
{
  return m_center;
}

bool AxisAlignedBoundingBox::isInside(const Eigen::Vector3f &pos) const noexcept
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

void AxisAlignedBoundingBox::translate(const Eigen::Vector3f &delta)
{
  m_center += delta;
}

} // namespace bsgo
