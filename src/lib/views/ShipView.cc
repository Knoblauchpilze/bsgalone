
#include "ShipView.hh"

namespace bsgo {

ShipView::ShipView(const Uuid &playerShipId, const CoordinatorShPtr &coordinator)
  : utils::CoreObject("ship")
  , m_playerShipId(playerShipId)
  , m_coordinator(coordinator)
{
  setService("view");
}

void ShipView::update(const float /*elapsedSeconds*/) {}

auto ShipView::getPlayerShip() const -> Entity
{
  return getShip(m_playerShipId);
}

auto ShipView::getShip(const Uuid &ship) const -> Entity
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
  std::vector<Entity> out;
  const auto uuids = m_coordinator->getEntitiesWithin(bbox, {EntityKind::SHIP});
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator->getEntity(uuid));
  }

  return out;
}

} // namespace bsgo
