
#include "MessageUtils.hh"

namespace bsgalone::client {

bool didPlayerShipDie(const core::EntityRemovedMessage &message,
                      const core::DatabaseEntityMapper &mapper)
{
  if (message.getEntityKind() != core::EntityKind::SHIP || !message.isDead())
  {
    return false;
  }

  const auto deadShipDbId        = message.getEntityDbId();
  const auto maybePlayerShipDbId = mapper.tryGetPlayerShipDbId();

  return maybePlayerShipDbId && deadShipDbId == *maybePlayerShipDbId;
}

bool didPlayerShipDie(const core::EntityRemovedMessage &message, const GameView &gameView)
{
  if (message.getEntityKind() != core::EntityKind::SHIP || !message.isDead())
  {
    return false;
  }

  return message.getEntityDbId() == gameView.getPlayerShipDbId();
}

bool didPlayerDie(const core::EntityRemovedMessage &message,
                  const core::DatabaseEntityMapper &mapper)
{
  if (message.getEntityKind() != core::EntityKind::PLAYER || !message.isDead())
  {
    return false;
  }

  const auto deadPlayerDbId  = message.getEntityDbId();
  const auto maybePlayerDbId = mapper.tryGetPlayerDbId();

  return maybePlayerDbId && deadPlayerDbId == *maybePlayerDbId;
}

} // namespace bsgalone::client
