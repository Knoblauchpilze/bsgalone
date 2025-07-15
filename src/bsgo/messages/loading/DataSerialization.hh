
#pragma once

#include "AsteroidData.hh"
#include "ComputerData.hh"
#include "OutpostData.hh"
#include "PlayerData.hh"
#include "ShipData.hh"
#include "SystemData.hh"
#include "WeaponData.hh"

namespace bsgo {

auto serializePlayerData(std::ostream &out, const PlayerData &data) -> std::ostream &;
bool deserializePlayerData(std::istream &in, PlayerData &data);

auto serializeAsteroidData(std::ostream &out, const AsteroidData &data) -> std::ostream &;
bool deserializeAsteroidData(std::istream &in, AsteroidData &data);

auto serializeComputerData(std::ostream &out, const ComputerData &data) -> std::ostream &;
bool deserializeComputerData(std::istream &in, ComputerData &data);

auto serializeWeaponData(std::ostream &out, const WeaponData &data) -> std::ostream &;
bool deserializeWeaponData(std::istream &in, WeaponData &data);

auto serializeOutpostData(std::ostream &out, const OutpostData &data) -> std::ostream &;
bool deserializeOutpostData(std::istream &in, OutpostData &data);

auto serializeShipData(std::ostream &out, const ShipData &data) -> std::ostream &;
bool deserializeShipData(std::istream &in, ShipData &data);

auto serializeSystemData(std::ostream &out, const SystemData &data) -> std::ostream &;
bool deserializeSystemData(std::istream &in, SystemData &data);

} // namespace bsgo
