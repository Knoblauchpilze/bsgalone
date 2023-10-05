
#pragma once

#include "Entity.hh"
#include "IBoundingBox.hh"
#include "MotionSystem.hh"
#include "Transform.hh"
#include "Uuid.hh"
#include "Velocity.hh"
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
  auto getTransform(const Uuid &ent) const -> std::optional<TransformShPtr>;
  auto getVelocity(const Uuid &ent) const -> std::optional<VelocityShPtr>;

  auto getEntityAt(const Eigen::Vector3f &pos) const -> std::optional<Uuid>;
  auto getEntitiesWithin(const IBoundingBox &bbox) const -> std::vector<Uuid>;

  private:
  std::unordered_map<Uuid, EntityKind> m_entities{};

  std::unordered_map<Uuid, TransformShPtr> m_transforms{};
  std::unordered_map<Uuid, VelocityShPtr> m_velocities{};

  MotionSystem m_motionSystem{};

  void addTransform(const Uuid &ent, IBoundingBoxPtr bbox);
  void addVelocity(const Uuid &ent, const Eigen::Vector3f &speed);
};

} // namespace bsgo
