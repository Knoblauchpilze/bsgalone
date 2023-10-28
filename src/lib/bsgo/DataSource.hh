
#pragma once

#include "Coordinator.hh"
#include "Repositories.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class DataSource : public utils::CoreObject
{
  public:
  DataSource();
  ~DataSource() override = default;

  auto playerId() const -> Uuid;
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

  void registerWeapon(Coordinator &coordinator, const Uuid &ship, const Uuid &weapon) const;
  void registerComputer(Coordinator &coordinator, const Uuid &ship, const Uuid &computer) const;
};

} // namespace bsgo
