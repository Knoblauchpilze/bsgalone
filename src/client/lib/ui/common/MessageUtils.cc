
#include "MessageUtils.hh"

namespace pge {

bool didPlayerShipDied(const bsgo::EntityRemovedMessage &message,
                       const bsgo::DatabaseEntityMapper &mapper)
{
  if (message.getEntityKind() != bsgo::EntityKind::SHIP || !message.isDead())
  {
    return false;
  }

  const auto deadShipDbId        = message.getEntityDbId();
  const auto maybePlayerShipDbId = mapper.tryGetPlayerShipDbId();

  return maybePlayerShipDbId && deadShipDbId == *maybePlayerShipDbId;
}

bool didPlayerShipDied(const bsgo::EntityRemovedMessage &message, const ShipDbView &shipDbView)
{
  if (message.getEntityKind() != bsgo::EntityKind::SHIP || !message.isDead())
  {
    return false;
  }

  return message.getEntityDbId() == shipDbView.getPlayerShipDbId();
}

} // namespace pge
