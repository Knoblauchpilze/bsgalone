
#pragma once

#include "IBoundingBox.hh"

namespace bsgalone::core {

struct TransformComponent
{
  IBoundingBoxShPtr bbox{};
  float heading{0.0f};

  /// @brief - Returns the extent of the bounding box. This represents the minimum dimension
  /// which allows, starting from the center, to cover the whole area of the bounding box
  /// attached to the transform component.
  /// @return - the size of the bounding box attached to the component
  auto size() const -> float;

  /// @brief - Transforms the input position expressed in the local coordinate frame to the
  /// transform component to a position expressed in the world coordinate frame.
  /// @param localPos - the local position to convert
  /// @return - the equivalent of the position in world coordinate
  auto transformToGlobal(const Eigen::Vector3f &localPos) const -> Eigen::Vector3f;
};

} // namespace bsgalone::core
