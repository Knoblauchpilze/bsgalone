
#pragma once

#include "IBoundingBox.hh"
#include <memory>

namespace bsgo {

class TransformComponent
{
  public:
  TransformComponent(IBoundingBoxShPtr bbox);
  ~TransformComponent() = default;

  auto position() const -> Eigen::Vector3f;
  auto size() const -> float;
  auto heading() const noexcept -> float;
  bool contains(const Eigen::Vector3f &pos) const noexcept;

  void translate(const Eigen::Vector3f &delta);
  void setHeading(const float heading);

  private:
  IBoundingBoxShPtr m_bbox{};
  float m_heading{};
};

using TransformComponentShPtr = std::shared_ptr<TransformComponent>;

} // namespace bsgo
