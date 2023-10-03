
#pragma once

#include "IBoundingBox.hh"

namespace bsgo {

class AxisAlignedBoundingBox : public IBoundingBox
{
  public:
  AxisAlignedBoundingBox(const Eigen::Vector3f &center, const Eigen::Vector3f &dims) noexcept;
  ~AxisAlignedBoundingBox() override = default;

  auto position() const -> Eigen::Vector3f override;
  bool isInside(const Eigen::Vector3f &pos) const noexcept override;

  void translate(const Eigen::Vector3f &delta) override;

  private:
  Eigen::Vector3f m_center{Eigen::Vector3f::Zero()};
  Eigen::Vector3f m_dims{Eigen::Vector3f::Ones()};
};

} // namespace bsgo
