
#pragma once

#include "AbstractSystem.hh"
#include "ComponentSyncMessage.hh"

namespace bsgo {

class NetworkSystem : public AbstractSystem
{
  public:
  NetworkSystem();
  ~NetworkSystem() override = default;

  protected:
  void updateEntity(Entity &entity, Coordinator &coordinator, const TickData &data) const override;

  private:
  void syncEntity(Entity &entity) const;
  bool syncComponent(const Entity &entity,
                     const ComponentType &type,
                     ComponentSyncMessage &message) const;

  bool syncHealthComponent(const Entity &entity, ComponentSyncMessage &message) const;
  bool syncPowerComponent(const Entity &entity, ComponentSyncMessage &message) const;
  bool syncStatusComponent(const Entity &entity, ComponentSyncMessage &message) const;
  bool syncTransformComponent(const Entity &entity, ComponentSyncMessage &message) const;
  bool syncVelocityComponent(const Entity &entity, ComponentSyncMessage &message) const;
};

} // namespace bsgo
