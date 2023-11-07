
#pragma once

#include "Components.hh"
#include "Entity.hh"
#include "IBoundingBox.hh"
#include "ISystem.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <core_utils/TimeUtils.hh>
#include <memory>
#include <optional>
#include <unordered_set>

namespace bsgo {

class DataSource;

class Coordinator : public utils::CoreObject
{
  public:
  Coordinator();
  ~Coordinator() override = default;

  auto createEntity(const EntityKind &kind) -> Uuid;

  void addTransform(const Uuid &ent, IBoundingBoxPtr bbox);
  void addVelocity(const Uuid &ent, const VelocityData &data);
  void addHealth(const Uuid &ent, const float hp, const float max, const float regen);
  void addPower(const Uuid &ent, const float power, const float max, const float regen);
  void addTarget(const Uuid &ent);
  void addTarget(const Uuid &ent, const Uuid &target);
  void addFaction(const Uuid &ent, const Faction &faction);
  void addLoot(const Uuid &ent);
  void addScanned(const Uuid &ent);
  void addOwner(const Uuid &ent, const Uuid &owner);
  void addDamage(const Uuid &ent, const float damage);
  void addRemoval(const Uuid &ent);
  void addStatus(const Uuid &ent, const Status &status);
  void addAI(const Uuid &ent, INodePtr behavior);
  void addWeapon(const Uuid &ent, const PlayerWeapon &weapon);
  void addComputer(const Uuid &ent, const PlayerComputer &computer);
  void addResourceComponent(const Uuid &ent, const Uuid &resource, const float amount);

  void addWeaponEffect(const Uuid &ent, const utils::Duration &duration, const float damageModifier);
  void removeEffect(const Uuid &ent, const EffectComponentShPtr &effect);

  auto getEntity(const Uuid &ent) const -> Entity;
  void deleteEntity(const Uuid &ent);

  auto getEntityAt(const Eigen::Vector3f &pos, const std::optional<EntityKind> &filter = {}) const
    -> std::optional<Uuid>;
  auto getEntitiesWithin(const IBoundingBox &bbox,
                         const std::optional<EntityKind> &filter = {}) const -> std::vector<Uuid>;

  using EntityPredicate = std::function<bool(const Entity &entity)>;
  auto getEntitiesSatistying(const EntityPredicate &predicate) const -> std::vector<Entity>;

  void update(float elapsedSeconds);

  private:
  Uuid m_nextEntity{Uuid(0)};
  std::unordered_set<Uuid> m_entities{};
  Components m_components{};
  std::vector<ISystemPtr> m_systems{};

  void createSystems();
  bool hasExpectedKind(const Uuid &ent, const std::optional<EntityKind> &kind) const;

  void checkEntityExist(const Uuid &ent, const std::string &componentName) const;
  template<typename ContainerType>
  void checkForOverrides(const Uuid &ent,
                         const std::string &componentName,
                         const ContainerType &components) const;

  void cleanUpDeadEntities();
};

using CoordinatorShPtr = std::shared_ptr<Coordinator>;

} // namespace bsgo
