
#include "ShipView.hh"

namespace bsgo {

ShipView::ShipView(const CoordinatorShPtr &coordinator, const Repositories &repositories)
  : utils::CoreObject("ship")
  , m_playerId(repositories.playerId)
  , m_coordinator(coordinator)
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

auto ShipView::getPlayerShip() const noexcept -> PlayerShip
{
  return getShip(m_playerId);
}

auto ShipView::getShip(const Uuid &player) const -> PlayerShip
{
  return m_playerShipRepo->findOneById(player);
}

auto ShipView::getPlayerShipEntity() const -> Entity
{
  return getShipEntity(m_playerId);
}

auto ShipView::getShipEntity(const Uuid &ship) const -> Entity
{
  const auto ent = m_coordinator->getEntity(ship);
  if (ent.kind != EntityKind::SHIP)
  {
    error("Expected " + str(ship) + " to have kind ship");
  }
  return ent;
}

auto ShipView::getShipsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>
{
  const auto uuids = m_coordinator->getEntitiesWithin(bbox, {EntityKind::SHIP});

  std::vector<Entity> out;
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator->getEntity(uuid));
  }

  return out;
}

} // namespace bsgo
