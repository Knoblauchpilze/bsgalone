
#pragma once

#include "DatabaseEntityMapper.hh"
#include "EntityRemovedMessage.hh"
#include "GameView.hh"

namespace bsgalone::client {

bool didPlayerShipDie(const core::EntityRemovedMessage &message,
                      const core::DatabaseEntityMapper &mapper);

bool didPlayerShipDie(const core::EntityRemovedMessage &message, const GameView &gameView);

bool didPlayerDie(const core::EntityRemovedMessage &message,
                  const core::DatabaseEntityMapper &mapper);

} // namespace bsgalone::client
