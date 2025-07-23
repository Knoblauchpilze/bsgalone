
#pragma once

#include "AsteroidData.hh"
#include "ComputerData.hh"
#include "OutpostData.hh"
#include "PlayerData.hh"
#include "ResourceData.hh"
#include "ShipData.hh"
#include "SystemData.hh"
#include "WeaponData.hh"
#include <gtest/gtest.h>

namespace bsgo {

template<typename T>
auto serializeAndDeserialize(const T &value) -> T;

void assertAsteroidDataAreEqual(const AsteroidData &actual, const AsteroidData &expected);
void assertComputerDataAreEqual(const ComputerData &actual, const ComputerData &expected);
void assertWeaponDataAreEqual(const WeaponData &actual, const WeaponData &expected);
void assertOutpostDataAreEqual(const OutpostData &actual, const OutpostData &expected);
void assertPlayerDataAreEqual(const PlayerData &actual, const PlayerData &expected);
void assertShipDataAreEqual(const ShipData &actual, const ShipData &expected);
void assertSystemDataAreEqual(const SystemData &actual, const SystemData &expected);
void assertResourceDataAreEqual(const ResourceData &actual, const ResourceData &expected);

} // namespace bsgo

#include "Common.hxx"
