
#pragma once

#include "DatabaseEntityMapper.hh"
#include "EntityRemovedMessage.hh"
#include "ShipDbView.hh"

namespace pge {

bool didPlayerShipDied(const bsgo::EntityRemovedMessage &message,
                       const bsgo::DatabaseEntityMapper &mapper);

bool didPlayerShipDied(const bsgo::EntityRemovedMessage &message, const ShipDbView &shipDbView);

} // namespace pge
