
#include "ShipView.hh"

namespace bsgo {

ShipView::ShipView(const Uuid &playerId, const CoordinatorShPtr &coordinator)
  : utils::CoreObject("ship")
  , m_playerId(playerId)
  , m_coordinator(coordinator)
{
  setService("view");
}

void ShipView::update(const float /*elapsedSeconds*/) {}

auto ShipView::getPlayerShip() const -> Entity
{
  return getShip(m_playerId);
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
  const auto uuids = m_coordinator->getEntitiesWithin(bbox, {EntityKind::SHIP});

  std::vector<Entity> out;
  for (const auto &uuid : uuids)
  {
    out.push_back(m_coordinator->getEntity(uuid));
  }

  return out;
}

} // namespace bsgo
