
#pragma once

#include "AsteroidData.hh"
#include "ComputerData.hh"
#include "IMessage.hh"
#include "OutpostData.hh"
#include "PlayerComputerData.hh"
#include "PlayerData.hh"
#include "PlayerResourceData.hh"
#include "PlayerShipData.hh"
#include "PlayerWeaponData.hh"
#include "ResourceData.hh"
#include "ShipData.hh"
#include "SystemData.hh"
#include "SystemTickData.hh"
#include "Target.hh"
#include "WeaponData.hh"

namespace bsgo {

void serializeAndDeserializeMessage(const bsgalone::core::IMessage &value,
                                    bsgalone::core::IMessage &output);

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
void assertWeaponDataAreEqual(const WeaponData &actual, const WeaponData &expected);
void assertComputerDataAreEqual(const ComputerData &actual, const ComputerData &expected);
void assertShipDataAreEqual(const ShipData &actual, const ShipData &expected);
void assertSystemTickDataAreEqual(const SystemTickData &actual, const SystemTickData &expected);
void assertTargetAreEqual(const bsgalone::core::Target &actual,
                          const bsgalone::core::Target &expected);

} // namespace bsgo
