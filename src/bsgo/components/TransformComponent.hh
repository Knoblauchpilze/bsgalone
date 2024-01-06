
#pragma once

#include "AbstractComponent.hh"
#include "IBoundingBox.hh"

namespace bsgo {

class TransformComponent : public AbstractComponent
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

  auto transformToGlobal(const Eigen::Vector3f &localPos) const -> Eigen::Vector3f;

  private:
  IBoundingBoxShPtr m_bbox{};
  float m_heading{};
};

using TransformComponentShPtr = std::shared_ptr<TransformComponent>;

} // namespace bsgo
