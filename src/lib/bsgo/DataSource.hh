
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
  DataSource(const Uuid &playerId);
  ~DataSource() override = default;

  void initialize(Coordinator &coordinator) const;

  private:
  Uuid m_playerId;

  AsteroidRepository m_asteroidRepo{};
  PlayerRepository m_playerRepo{};
  SystemRepository m_systemRepo{};
  PlayerShipRepository m_playerShipRepo{};
  OutpostRepository m_outpostRepo{};
};

} // namespace bsgo
