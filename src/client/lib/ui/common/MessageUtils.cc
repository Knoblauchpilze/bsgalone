
#include "MessageUtils.hh"

namespace pge {

bool didPlayerShipDied(const bsgo::EntityRemovedMessage &message,
                       const bsgo::DatabaseEntityMapper &mapper)
{
  if (message.getEntityKind() != bsgo::EntityKind::SHIP)
  {
    return false;
  }

  const auto deadShipDbId        = message.getEntityDbId();
  const auto maybePlayerShipDbId = mapper.tryGetPlayerShipDbId();

  return maybePlayerShipDbId && deadShipDbId == *maybePlayerShipDbId;
}

bool didPlayerShipDied(const bsgo::EntityRemovedMessage &message, const bsgo::ShipDbView &shipDbView)
{
  if (message.getEntityKind() != bsgo::EntityKind::SHIP)
  {
    return false;
  }

  return message.getEntityDbId() == shipDbView.getPlayerShipDbId();
}

} // namespace pge
