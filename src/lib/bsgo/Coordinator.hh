
#pragma once

#include "Entity.hh"
#include "IBoundingBox.hh"
#include "TransformSystem.hh"
#include "Uuid.hh"
#include "VelocitySystem.hh"
#include <core_utils/CoreObject.hh>
#include <unordered_map>

namespace bsgo {

class Coordinator : public utils::CoreObject
{
  public:
  Coordinator();
  virtual ~Coordinator() = default;

  auto createEntity(const EntityKind &kind) -> Uuid;
  auto createEntity(const EntityKind &kind, IBoundingBoxPtr bbox) -> Uuid;
  auto createEntity(const EntityKind &kind, IBoundingBoxPtr bbox, const Eigen::Vector3f &speed)
    -> Uuid;
  auto getEntity(const Uuid &ent) const -> Entity;

  auto getTransformSystem() -> TransformSystem &;
  auto getTransformSystem() const -> const TransformSystem &;

  auto getVelocitySystem() -> VelocitySystem &;
  auto getVelocitySystem() const -> const VelocitySystem &;

  private:
  std::unordered_map<Uuid, EntityKind> m_entities{};
  TransformSystem m_transformSystem{};
  VelocitySystem m_velocitySystem{};
};

} // namespace bsgo
