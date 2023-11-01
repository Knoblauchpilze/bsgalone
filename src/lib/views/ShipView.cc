
#include "ShipView.hh"

namespace bsgo {

ShipView::ShipView(const Uuid &playerShipId,
                   const CoordinatorShPtr &coordinator,
                   const Repositories &repositories)
  : IView("ship", coordinator, repositories)
  , m_playerShipId(playerShipId)
{}

auto ShipView::getPlayerShip() const -> Entity
{
  const auto ent = m_coordinator->getEntity(m_playerShipId);
  if (ent.kind->kind() != EntityKind::SHIP)
  {
    error("Expected " + str(m_playerShipId) + " to have kind ship but got " + ent.str());
  }
  return ent;
}

bool ShipView::hasTarget() const
{
  return getPlayerTarget().has_value();
}

auto ShipView::getPlayerTarget() const -> std::optional<Entity>
{
  const auto ent      = m_coordinator->getEntity(m_playerShipId);
  const auto targetId = ent.targetComp().target();
  if (!targetId)
  {
    return {};
  }

  return {m_coordinator->getEntity(*targetId)};
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

void ShipView::tryActivateWeapon(const Uuid &ship, const int weaponId)
{
  if (ship != m_playerShipId)
  {
    error("Failed to activate weapon " + std::to_string(weaponId),
          "Expected ship " + str(m_playerShipId) + " but got " + str(ship));
  }

  auto data = getPlayerShip();
  if (data.weapons.size() < static_cast<std::size_t>(weaponId))
  {
    error("Failed to activate weapon " + std::to_string(weaponId),
          "Ship only has " + std::to_string(data.weapons.size()) + " weapon(s)");
  }

  const auto weapon = data.weapons[weaponId];
  weapon->toggle();
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
  computer->registerFireRequest();
}

} // namespace bsgo
