
#pragma once

#include "PlayerShipData.hh"
#include "ShipData.hh"

namespace bsgo {

auto serializePlayerShipData(std::ostream &out, const PlayerShipData &data) -> std::ostream &;
bool deserializePlayerShipData(std::istream &in, PlayerShipData &data);

auto serializeShipData(std::ostream &out, const ShipData &data) -> std::ostream &;
bool deserializeShipData(std::istream &in, ShipData &data);

} // namespace bsgo
