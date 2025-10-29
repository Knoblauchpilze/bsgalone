
#pragma once

#include "PlayerShipData.hh"
#include "ShipData.hh"
#include "SystemData.hh"
#include "SystemTickData.hh"

namespace bsgo {

auto serializePlayerShipData(std::ostream &out, const PlayerShipData &data) -> std::ostream &;
bool deserializePlayerShipData(std::istream &in, PlayerShipData &data);

auto serializeSystemData(std::ostream &out, const SystemData &data) -> std::ostream &;
bool deserializeSystemData(std::istream &in, SystemData &data);

auto serializeShipData(std::ostream &out, const ShipData &data) -> std::ostream &;
bool deserializeShipData(std::istream &in, ShipData &data);

auto serializeSystemTickData(std::ostream &out, const SystemTickData &data) -> std::ostream &;
bool deserializeSystemTickData(std::istream &in, SystemTickData &data);

} // namespace bsgo
