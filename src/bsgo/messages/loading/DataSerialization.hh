
#pragma once

#include "PlayerShipData.hh"

namespace bsgo {

auto serializePlayerShipData(std::ostream &out, const PlayerShipData &data) -> std::ostream &;
bool deserializePlayerShipData(std::istream &in, PlayerShipData &data);

} // namespace bsgo
