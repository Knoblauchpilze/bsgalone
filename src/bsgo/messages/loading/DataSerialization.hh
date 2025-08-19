
#pragma once

#include "AsteroidData.hh"
#include "ComputerData.hh"
#include "OutpostData.hh"
#include "PlayerComputerData.hh"
#include "PlayerData.hh"
#include "PlayerResourceData.hh"
#include "PlayerShipData.hh"
#include "PlayerWeaponData.hh"
#include "ResourceData.hh"
#include "ShipData.hh"
#include "SystemData.hh"
#include "WeaponData.hh"

namespace bsgo {

auto serializePlayerData(std::ostream &out, const PlayerData &data) -> std::ostream &;
bool deserializePlayerData(std::istream &in, PlayerData &data);

auto serializeAsteroidData(std::ostream &out, const AsteroidData &data) -> std::ostream &;
bool deserializeAsteroidData(std::istream &in, AsteroidData &data);

auto serializePlayerComputerData(std::ostream &out, const PlayerComputerData &data)
  -> std::ostream &;
bool deserializePlayerComputerData(std::istream &in, PlayerComputerData &data);

auto serializePlayerWeaponData(std::ostream &out, const PlayerWeaponData &data) -> std::ostream &;
bool deserializePlayerWeaponData(std::istream &in, PlayerWeaponData &data);

auto serializeOutpostData(std::ostream &out, const OutpostData &data) -> std::ostream &;
bool deserializeOutpostData(std::istream &in, OutpostData &data);

auto serializePlayerShipData(std::ostream &out, const PlayerShipData &data) -> std::ostream &;
bool deserializePlayerShipData(std::istream &in, PlayerShipData &data);

auto serializeSystemData(std::ostream &out, const SystemData &data) -> std::ostream &;
bool deserializeSystemData(std::istream &in, SystemData &data);

auto serializeResourceData(std::ostream &out, const ResourceData &data) -> std::ostream &;
bool deserializeResourceData(std::istream &in, ResourceData &data);

auto serializePlayerResourceData(std::ostream &out, const PlayerResourceData &data)
  -> std::ostream &;
bool deserializePlayerResourceData(std::istream &in, PlayerResourceData &data);

auto serializeWeaponData(std::ostream &out, const WeaponData &data) -> std::ostream &;
bool deserializeWeaponData(std::istream &in, WeaponData &data);

auto serializeComputerData(std::ostream &out, const ComputerData &data) -> std::ostream &;
bool deserializeComputerData(std::istream &in, ComputerData &data);

auto serializeShipData(std::ostream &out, const ShipData &data) -> std::ostream &;
bool deserializeShipData(std::istream &in, ShipData &data);

} // namespace bsgo
