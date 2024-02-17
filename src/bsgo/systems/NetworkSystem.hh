
#pragma once

#include "AbstractSystem.hh"
#include "ComponentSyncMessage.hh"
#include "Repositories.hh"

namespace bsgo {

class NetworkSystem : public AbstractSystem
{
  public:
  NetworkSystem(const Repositories &repositories);
  ~NetworkSystem() override = default;

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;

  private:
  Repositories m_repositories{};

  void syncEntity(Entity &entity) const;
  bool syncComponent(const Entity &entity,
                     const ComponentType &type,
                     ComponentSyncMessage &message) const;
  bool syncStatusComponent(const Entity &entity, ComponentSyncMessage &message) const;
  bool syncTransformComponent(const Entity &entity, ComponentSyncMessage &message) const;
  bool syncVelocityComponent(const Entity &entity, ComponentSyncMessage &message) const;
};

} // namespace bsgo
