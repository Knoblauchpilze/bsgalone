
#include "ShipView.hh"

namespace bsgo {

ShipView::ShipView(const Uuid &playerShipId, const CoordinatorShPtr &coordinator)
  : IView("ship")
  , m_playerShipId(playerShipId)
  , m_coordinator(coordinator)
{}

auto ShipView::getPlayerShip() const -> Entity
{
  return getShip(m_playerShipId);
}

auto ShipView::getShip(const Uuid &ship) const -> Entity
{
  const auto ent = m_coordinator->getEntity(ship);
  if (ent.kind != EntityKind::SHIP)
  {
    error("Expected " + str(ship) + " to have kind ship but got " + ent.str());
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
