
#include "ShipView.hh"

namespace bsgo {

ShipView::ShipView(const Uuid &playerShipId, const CoordinatorShPtr &coordinator)
  : IView("ship", coordinator)
  , m_playerShipId(playerShipId)
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

auto ShipView::getPlayerTarget() const -> std::optional<Entity>
{
  return getTarget(m_playerShipId);
}

auto ShipView::distanceToTarget() const -> float
{
  const auto target = getPlayerTarget();
  if (!target)
  {
    error("Can't get distance to non existing target");
  }

  const auto playerShip = getPlayerShip();
  const auto playerPos  = playerShip.transformComp().position();
  const auto targetPos  = target->transformComp().position();

  return (targetPos - playerPos).norm();
}

auto ShipView::getTarget(const Uuid &ship) const -> std::optional<Entity>
{
  const auto ent      = getShip(ship);
  const auto targetId = ent.targetComp().target();
  if (!targetId)
  {
    return {};
  }

  return {getEntity(*targetId)};
}

} // namespace bsgo
