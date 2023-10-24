
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
  if (ent.kind->kind() != EntityKind::SHIP)
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

void ShipView::tryActivateSlot(const Uuid &ship, const int slotId)
{
  if (ship != m_playerShipId)
  {
    error("Failed to activate slot " + std::to_string(slotId),
          "Expected ship " + str(m_playerShipId) + " but got " + str(ship));
  }

  auto data = getPlayerShip();
  if (data.computers.size() < static_cast<std::size_t>(slotId))
  {
    error("Failed to activate slot " + std::to_string(slotId),
          "Ship only has " + std::to_string(data.computers.size()) + " computer(s)");
  }

  const auto computer = data.computers[slotId];
  if (!computer->canFire())
  {
    return;
  }
  const auto damage = computer->damageModifier();
  if (damage)
  {
    const auto duration = computer->duration();
    if (!duration)
    {
      error("Failed to activate slot " + std::to_string(slotId),
            "Expected slot to define a duration");
    }
    log("Adding weapon effect for " + data.str() + " with duration "
        + utils::durationToString(*duration));
    m_coordinator->addWeaponEffect(ship, *duration, *damage);
  }

  computer->fire();

  const auto powerUsed = computer->powerCost();
  data.powerComp().usePower(powerUsed);
}

} // namespace bsgo
