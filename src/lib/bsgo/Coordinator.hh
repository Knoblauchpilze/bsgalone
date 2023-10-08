
#pragma once

#include "Components.hh"
#include "Entity.hh"
#include "IBoundingBox.hh"
#include "MotionSystem.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <memory>
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

  void update(float elapsedSeconds);

  private:
  std::unordered_map<Uuid, EntityKind> m_entities{};
  Components m_components{};
  MotionSystem m_motionSystem{};

  void addTransform(const Uuid &ent, IBoundingBoxPtr bbox);
  void addVelocity(const Uuid &ent, const Eigen::Vector3f &speed);
};

using CoordinatorShPtr = std::shared_ptr<Coordinator>;

} // namespace bsgo
