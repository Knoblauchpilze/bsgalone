
#pragma once

#include "IBoundingBox.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class CircleBox : public IBoundingBox
{
  public:
  CircleBox(const Eigen::Vector3f &center, const float radius) noexcept;
  ~CircleBox() override = default;

  auto position() const -> Eigen::Vector3f override;
  bool isInside(const Eigen::Vector3f &pos) const noexcept override;

  void translate(const Eigen::Vector3f &delta) override;

  private:
  Eigen::Vector3f m_center{Eigen::Vector3f::Zero()};
  float m_radius{1.0f};
  float m_squaredRadius{1.0f};
};

} // namespace bsgo
