
#include "ShipDiedMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const ShipDiedMessage &actual, const ShipDiedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_ShipDiedMessage, Basic)
{
  const ShipDiedMessage expected(Uuid{789}, Uuid{666});
  ShipDiedMessage actual(Uuid{10}, Uuid{10001});
  actual.validate();
  actual.setClientId(Uuid{47});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ShipDiedMessage, Validated)
{
  ShipDiedMessage expected(Uuid{789}, Uuid{666});
  expected.validate();
  ShipDiedMessage actual(Uuid{10}, Uuid{10001});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ShipDiedMessage, WithClientId)
{
  ShipDiedMessage expected(Uuid{789}, Uuid{666});
  expected.setClientId(Uuid{36});
  ShipDiedMessage actual(Uuid{10}, Uuid{10001});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ShipDiedMessage, Clone)
{
  ShipDiedMessage expected(Uuid{789}, Uuid{666});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::SHIP_DIED);
  assertMessagesAreEqual(cloned->as<ShipDiedMessage>(), expected);
}

} // namespace bsgo
