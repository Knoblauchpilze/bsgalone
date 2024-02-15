
#include "ComponentSyncMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const ComponentSyncMessage &actual, const ComponentSyncMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, Basic)
{
  const ComponentSyncMessage expected(Uuid{987654}, Uuid{123456});
  ComponentSyncMessage actual(Uuid{44}, Uuid{24});
  actual.setClientId(Uuid{89765});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, WithClientId)
{
  ComponentSyncMessage expected(Uuid{987654}, Uuid{123456});
  expected.setClientId(Uuid{56789});
  ComponentSyncMessage actual(Uuid{44}, Uuid{24});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ComponentSyncMessage, Clone)
{
  ComponentSyncMessage expected(Uuid{987654}, Uuid{123456});
  expected.setClientId(Uuid{56789});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::COMPONENT_SYNC);
  assertMessagesAreEqual(cloned->as<ComponentSyncMessage>(), expected);
}

} // namespace bsgo
