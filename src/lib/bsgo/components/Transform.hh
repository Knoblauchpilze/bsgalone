
#pragma once

#include "IBoundingBox.hh"
#include <memory>

namespace bsgo {

class Transform
{
  public:
  Transform() = default;
  Transform(IBoundingBoxShPtr bbox);
  ~Transform() = default;

  auto position() const -> Eigen::Vector3f;
  bool contains(const Eigen::Vector3f &pos) const noexcept;

  void translate(const Eigen::Vector3f &delta);

  private:
  IBoundingBoxShPtr m_bbox{};
};

using TransformShPtr = std::shared_ptr<Transform>;

} // namespace bsgo
