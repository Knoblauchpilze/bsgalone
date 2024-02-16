
#pragma once

#include "AbstractSystem.hh"
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
  void syncComponent(Entity &entity, const ComponentType &type) const;

  void syncStatusComponent(const Entity &entity) const;
};

} // namespace bsgo
