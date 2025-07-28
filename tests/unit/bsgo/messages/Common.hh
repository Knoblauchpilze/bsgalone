
#pragma once

#include "AsteroidData.hh"
#include "OutpostData.hh"
#include "PlayerComputerData.hh"
#include "PlayerData.hh"
#include "PlayerResourceData.hh"
#include "PlayerShipData.hh"
#include "PlayerWeaponData.hh"
#include "ResourceData.hh"
#include "SystemData.hh"
#include <gtest/gtest.h>

namespace bsgo {

template<typename T>
auto serializeAndDeserialize(const T &value) -> T;

void assertAsteroidDataAreEqual(const AsteroidData &actual, const AsteroidData &expected);
void assertPlayerComputerDataAreEqual(const PlayerComputerData &actual,
                                      const PlayerComputerData &expected);
void assertPlayerWeaponDataAreEqual(const PlayerWeaponData &actual,
                                    const PlayerWeaponData &expected);
void assertOutpostDataAreEqual(const OutpostData &actual, const OutpostData &expected);
void assertPlayerDataAreEqual(const PlayerData &actual, const PlayerData &expected);
void assertPlayerShipDataAreEqual(const PlayerShipData &actual, const PlayerShipData &expected);
void assertSystemDataAreEqual(const SystemData &actual, const SystemData &expected);
void assertResourceDataAreEqual(const ResourceData &actual, const ResourceData &expected);
void assertPlayerResourceDataAreEqual(const PlayerResourceData &actual,
                                      const PlayerResourceData &expected);

} // namespace bsgo

#include "Common.hxx"
