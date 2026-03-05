
#include "MessageUtils.hh"

namespace pge {

bool didPlayerShipDie(const bsgo::EntityRemovedMessage &message,
                      const bsgo::DatabaseEntityMapper &mapper)
{
  if (message.getEntityKind() != bsgalone::core::EntityKind::SHIP || !message.isDead())
  {
    return false;
  }

  const auto deadShipDbId        = message.getEntityDbId();
  const auto maybePlayerShipDbId = mapper.tryGetPlayerShipDbId();

  return maybePlayerShipDbId && deadShipDbId == *maybePlayerShipDbId;
}

bool didPlayerShipDie(const bsgo::EntityRemovedMessage &message, const ShipDbView &shipDbView)
{
  if (message.getEntityKind() != bsgalone::core::EntityKind::SHIP || !message.isDead())
  {
    return false;
  }

  return message.getEntityDbId() == shipDbView.getPlayerShipDbId();
}

bool didPlayerDie(const bsgo::EntityRemovedMessage &message,
                  const bsgo::DatabaseEntityMapper &mapper)
{
  if (message.getEntityKind() != bsgalone::core::EntityKind::PLAYER || !message.isDead())
  {
    return false;
  }

  const auto deadPlayerDbId  = message.getEntityDbId();
  const auto maybePlayerDbId = mapper.tryGetPlayerDbId();

  return maybePlayerDbId && deadPlayerDbId == *maybePlayerDbId;
}

} // namespace pge
