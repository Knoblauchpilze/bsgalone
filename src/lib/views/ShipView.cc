
#include "ShipView.hh"

namespace bsgo {

ShipView::ShipView(const Repositories &repositories)
  : utils::CoreObject("ship")
  , m_playerId(repositories.playerId)
  , m_playerRepo(repositories.player)
  , m_playerShipRepo(repositories.playerShip)
{
  setService("view");
  if (nullptr == m_playerRepo)
  {
    throw std::invalid_argument("Expected non null player repository");
  }
  if (nullptr == m_playerShipRepo)
  {
    throw std::invalid_argument("Expected non null player ship repository");
  }
}

void ShipView::update(const float /*elapsedSeconds*/) {}

auto ShipView::getPlayerShipId() const noexcept -> Uuid
{
  return m_playerRepo->findShipById(m_playerId);
}

auto ShipView::getPlayerShip(const Uuid &player) const -> PlayerShip
{
  return m_playerShipRepo->findOneById(player);
}

} // namespace bsgo
