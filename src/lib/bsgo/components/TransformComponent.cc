
#include "TransformComponent.hh"
#include "AxisAlignedBoundingBox.hh"
#include "CircleBox.hh"
#include <cxxabi.h>

namespace bsgo {

TransformComponent::TransformComponent(IBoundingBoxShPtr bbox)
  : AbstractComponent(ComponentType::TRANSFORM)
  , m_bbox(std::move(bbox))
{}

auto TransformComponent::position() const -> Eigen::Vector3f
{
  return m_bbox->position();
}

auto TransformComponent::size() const -> float
{
  if (!m_bbox)
  {
    return 0.0f;
  }

  if (const auto circleBox = std::dynamic_pointer_cast<CircleBox>(m_bbox); circleBox != nullptr)
  {
    return circleBox->radius();
  }
  if (const auto aabb = std::dynamic_pointer_cast<AxisAlignedBoundingBox>(m_bbox); aabb != nullptr)
  {
    return aabb->dims().maxCoeff();
  }

  /// https://stackoverflow.com/questions/3649278/how-can-i-get-the-class-name-from-a-c-object
  int status;
  std::string name = ::abi::__cxa_demangle(typeid(*m_bbox).name(), nullptr, nullptr, &status);

  throw std::invalid_argument("Unsupported bounding box type " + name);
}

auto TransformComponent::heading() const noexcept -> float
{
  return m_heading;
}

bool TransformComponent::contains(const Eigen::Vector3f &pos) const noexcept
{
  return m_bbox && m_bbox->isInside(pos);
}

void TransformComponent::translate(const Eigen::Vector3f &delta)
{
  m_bbox->translate(delta);
}

void TransformComponent::setHeading(const float heading)
{
  m_heading = heading;
}

void TransformComponent::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
