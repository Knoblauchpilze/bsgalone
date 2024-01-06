
#pragma once

#include "AbstractSystem.hh"
#include "Repositories.hh"
#include <optional>

namespace bsgo {

class NetworkSystem : public AbstractSystem
{
  public:
  NetworkSystem(const Repositories &repositories);
  ~NetworkSystem() override = default;

  void setPlayerDbId(const Uuid &playerDbId);

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;

  private:
  std::optional<Uuid> m_playerDbId{};
  Repositories m_repositories{};

  void syncEntity(Entity &entity) const;
  void syncComponent(Entity &entity, const ComponentType &type) const;

  void syncResourceComponents(Entity &entity) const;
};

} // namespace bsgo
