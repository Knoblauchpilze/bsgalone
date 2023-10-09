
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

auto ShipView::getShip(const Uuid &player) const -> PlayerShip
{
  return m_playerShipRepo->findOneById(player);
}

bool ShipView::hasTarget() const noexcept
{
  return m_target.has_value();
}

void ShipView::setTarget(const Uuid &uuid)
{
  m_target = uuid;
}

void ShipView::clearTarget()
{
  m_target.reset();
}

auto ShipView::getTargetHealth() const noexcept -> float
{
  return 234.0f;
}

auto ShipView::getTargetMaxHealth() const noexcept -> float
{
  return 7698.7f;
}

auto ShipView::getTargetPower() const noexcept -> float
{
  return 33.8f;
}

auto ShipView::getTargetMaxPower() const noexcept -> float
{
  return 35.1f;
}

} // namespace bsgo
