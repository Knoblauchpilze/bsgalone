
#pragma once

#include <eigen3/Eigen/Eigen>
#include <memory>

namespace bsgo {

class IBoundingBox
{
  public:
  virtual ~IBoundingBox() = default;

  virtual auto position() const -> Eigen::Vector3f                 = 0;
  virtual bool isInside(const Eigen::Vector3f &pos) const noexcept = 0;

  virtual void translate(const Eigen::Vector3f &delta) = 0;
};

using IBoundingBoxPtr   = std::unique_ptr<IBoundingBox>;
using IBoundingBoxShPtr = std::shared_ptr<IBoundingBox>;

} // namespace bsgo
