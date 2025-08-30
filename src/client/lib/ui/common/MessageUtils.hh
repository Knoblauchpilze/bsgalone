
#pragma once

#include "DatabaseEntityMapper.hh"
#include "EntityRemovedMessage.hh"
#include "ShipDbView.hh"

namespace pge {

bool didPlayerShipDie(const bsgo::EntityRemovedMessage &message,
                      const bsgo::DatabaseEntityMapper &mapper);

bool didPlayerShipDie(const bsgo::EntityRemovedMessage &message, const ShipDbView &shipDbView);

bool didPlayerDie(const bsgo::EntityRemovedMessage &message,
                  const bsgo::DatabaseEntityMapper &mapper);

} // namespace pge
