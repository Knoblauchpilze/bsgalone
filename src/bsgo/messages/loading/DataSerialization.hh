
#pragma once

#include "AsteroidData.hh"
#include "ComputerData.hh"
#include "WeaponData.hh"

namespace bsgo {

auto serializeAsteroidData(std::ostream &out, const AsteroidData &data) -> std::ostream &;
bool deserializeAsteroidData(std::istream &in, AsteroidData &data);

auto serializeComputerData(std::ostream &out, const ComputerData &data) -> std::ostream &;
bool deserializeComputerData(std::istream &in, ComputerData &data);

auto serializeWeaponData(std::ostream &out, const WeaponData &data) -> std::ostream &;
bool deserializeWeaponData(std::istream &in, WeaponData &data);

} // namespace bsgo
