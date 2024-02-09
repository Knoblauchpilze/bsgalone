
#pragma once

#include "DatabaseEntityMapper.hh"
#include "EntityDiedMessage.hh"
#include "ShipDbView.hh"

namespace pge {

bool didPlayerShipDied(const bsgo::EntityDiedMessage &message,
                       const bsgo::DatabaseEntityMapper &mapper);

bool didPlayerShipDied(const bsgo::EntityDiedMessage &message, const bsgo::ShipDbView &shipDbView);

} // namespace pge
