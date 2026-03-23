
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
#include "System.hh"
#include "Target.hh"
#include "WeaponData.hh"

namespace test {

void assertAsteroidDataAreEqual(const bsgalone::core::AsteroidData &actual,
                                const bsgalone::core::AsteroidData &expected);
void assertPlayerComputerDataAreEqual(const bsgalone::core::PlayerComputerData &actual,
                                      const bsgalone::core::PlayerComputerData &expected);
void assertPlayerWeaponDataAreEqual(const bsgalone::core::PlayerWeaponData &actual,
                                    const bsgalone::core::PlayerWeaponData &expected);
void assertOutpostDataAreEqual(const bsgalone::core::OutpostData &actual,
                               const bsgalone::core::OutpostData &expected);
void assertPlayerDataAreEqual(const bsgalone::core::PlayerData &actual,
                              const bsgalone::core::PlayerData &expected);
void assertPlayerShipDataAreEqual(const bsgalone::core::PlayerShipData &actual,
                                  const bsgalone::core::PlayerShipData &expected);
void assertSystemAreEqual(const bsgalone::core::System &actual,
                          const bsgalone::core::System &expected);
void assertResourceDataAreEqual(const bsgalone::core::ResourceData &actual,
                                const bsgalone::core::ResourceData &expected);
void assertPlayerResourceDataAreEqual(const bsgalone::core::PlayerResourceData &actual,
                                      const bsgalone::core::PlayerResourceData &expected);
void assertWeaponDataAreEqual(const bsgalone::core::WeaponData &actual,
                              const bsgalone::core::WeaponData &expected);
void assertComputerDataAreEqual(const bsgalone::core::ComputerData &actual,
                                const bsgalone::core::ComputerData &expected);
void assertShipDataAreEqual(const bsgalone::core::ShipData &actual,
                            const bsgalone::core::ShipData &expected);
void assertTargetAreEqual(const bsgalone::core::Target &actual,
                          const bsgalone::core::Target &expected);

} // namespace test
