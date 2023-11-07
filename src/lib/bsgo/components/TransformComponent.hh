
#pragma once

#include "IBoundingBox.hh"
#include "IComponent.hh"

namespace bsgo {

class TransformComponent : public IComponent
{
  public:
  TransformComponent(IBoundingBoxShPtr bbox);
  ~TransformComponent() override = default;

  auto position() const -> Eigen::Vector3f;
  auto size() const -> float;
  auto heading() const noexcept -> float;
  bool contains(const Eigen::Vector3f &pos) const noexcept;

  void translate(const Eigen::Vector3f &delta);
  void setHeading(const float heading);

  void update(const float elapsedSeconds) override;

  private:
  IBoundingBoxShPtr m_bbox{};
  float m_heading{};
};

using TransformComponentShPtr = std::shared_ptr<TransformComponent>;

} // namespace bsgo
