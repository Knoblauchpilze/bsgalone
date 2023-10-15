
#pragma once

#include "AsteroidRepository.hh"
#include "Coordinator.hh"
#include "OutpostRepository.hh"
#include "PlayerRepository.hh"
#include "PlayerShipRepository.hh"
#include "SystemRepository.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class DataSource : public utils::CoreObject
{
  public:
  DataSource();
  ~DataSource() override = default;

  auto playerId() const -> Uuid;
  auto playerShipId() const -> Uuid;

  void initialize(Coordinator &coordinator) const;

  private:
  Uuid m_playerId{};

  AsteroidRepository m_asteroidRepo{};
  PlayerRepository m_playerRepo{};
  SystemRepository m_systemRepo{};
  PlayerShipRepository m_playerShipRepo{};
  OutpostRepository m_outpostRepo{};

  void initializeAsteroids(Coordinator &coordinator, const Uuid &system) const;
  void initializeShips(Coordinator &coordinator, const Uuid &system) const;
  void initializeOutposts(Coordinator &coordinator, const Uuid &system) const;
};

} // namespace bsgo
