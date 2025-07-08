
#include "WeaponComponentMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const WeaponComponentMessage &actual,
                            const WeaponComponentMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getComponentType(), expected.getComponentType());
  EXPECT_EQ(actual.getWeaponDbId(), expected.getWeaponDbId());
  EXPECT_EQ(actual.isActive(), expected.isActive());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_WeaponComponentMessage, Basic)
{
  const WeaponComponentMessage expected(Uuid{14}, Uuid{2}, false);
  WeaponComponentMessage actual(Uuid{36}, Uuid{1}, true);
  actual.setClientId(Uuid{44});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_WeaponComponentMessage, WithClientId)
{
  WeaponComponentMessage expected(Uuid{28}, Uuid{67}, true);
  expected.setClientId(Uuid{119});
  WeaponComponentMessage actual(Uuid{51}, Uuid{180}, false);
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_WeaponComponentMessage, Clone)
{
  const WeaponComponentMessage expected(Uuid{28}, Uuid{67}, true);
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::WEAPON_COMPONENT_UPDATED);
  assertMessagesAreEqual(cloned->as<WeaponComponentMessage>(), expected);
}

} // namespace bsgo
