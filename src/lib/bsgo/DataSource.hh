
#pragma once

#include "Repositories.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class Coordinator;

class DataSource : public utils::CoreObject
{
  public:
  DataSource();
  ~DataSource() override = default;

  auto repositories() const -> Repositories;
  auto playerId() const -> Uuid;
  auto playerEntityId() const -> Uuid;
  auto playerShipEntityId() const -> Uuid;

  void initialize(Coordinator &coordinator) const;

  private:
  Uuid m_playerId{};
  mutable std::optional<Uuid> m_playerEntityId{};
  mutable std::optional<Uuid> m_playerShipEntityId{};

  Repositories m_repositories{};

  void initializePlayer(Coordinator &coordinator) const;
  void initializeAsteroids(Coordinator &coordinator, const Uuid &system) const;
  void initializeShips(Coordinator &coordinator, const Uuid &system) const;
  void initializeOutposts(Coordinator &coordinator, const Uuid &system) const;
  void registerAsteroid(Coordinator &coordinator, const Uuid &asteroid) const;
  void registerShip(Coordinator &coordinator, const Uuid &ship) const;
  void registerOutpost(Coordinator &coordinator, const Uuid &outpost) const;

  void registerShipWeapons(Coordinator &coordinator, const Uuid &ship, const Uuid &shipEntity) const;
  void registerShipComputers(Coordinator &coordinator,
                             const Uuid &ship,
                             const Uuid &shipEntity) const;
};

} // namespace bsgo
