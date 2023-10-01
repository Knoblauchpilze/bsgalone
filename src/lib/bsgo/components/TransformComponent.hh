
#pragma once

#include "IBoundingBox.hh"

namespace bsgo {

class TransformComponent
{
  public:
  TransformComponent() = default;
  TransformComponent(IBoundingBoxShPtr bbox);

  auto position() const -> Eigen::Vector3f;
  bool contains(const float x, const float y) const noexcept;

  private:
  IBoundingBoxShPtr m_bbox{};
};

} // namespace bsgo
