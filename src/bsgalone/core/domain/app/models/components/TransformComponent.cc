
#include "TransformComponent.hh"
#include "AxisAlignedBox.hh"
#include "CircleBox.hh"
#include <cxxabi.h>

namespace bsgalone::core {

auto TransformComponent::size() const -> float
{
  if (!bbox)
  {
    return 0.0f;
  }

  if (const auto circleBox = std::dynamic_pointer_cast<CircleBox>(bbox); circleBox != nullptr)
  {
    return circleBox->radius();
  }
  if (const auto aabb = std::dynamic_pointer_cast<AxisAlignedBox>(bbox); aabb != nullptr)
  {
    return aabb->dims().maxCoeff();
  }

  /// https://stackoverflow.com/questions/3649278/how-can-i-get-the-class-name-from-a-c-object
  int status;
  std::string name = ::abi::__cxa_demangle(typeid(*bbox).name(), nullptr, nullptr, &status);

  throw std::invalid_argument("Unsupported bounding box type " + name);
}

namespace {
const Eigen::Vector3f Z_AXIS = Eigen::Vector3f(0.0, 0.0, 1.0);
}

auto TransformComponent::transformToGlobal(const Eigen::Vector3f &localPos) const -> Eigen::Vector3f
{
  const Eigen::AngleAxisf rotation(-heading, Z_AXIS);
  const Eigen::Vector3f offset = rotation * localPos;
  return bbox->position() + offset;
}

} // namespace bsgalone::core
