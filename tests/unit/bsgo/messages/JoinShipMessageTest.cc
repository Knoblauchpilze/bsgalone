
#include "JoinShipMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const JoinShipMessage &actual, const JoinShipMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_JoinShipMessage, Basic)
{
  const JoinShipMessage expected(Uuid{14}, Uuid{17});
  JoinShipMessage actual(Uuid{37}, Uuid{879871});
  actual.setClientId(Uuid{5});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JoinShipMessage, WithClientId)
{
  JoinShipMessage expected(Uuid{123}, Uuid{324});
  expected.setClientId(Uuid{119});
  JoinShipMessage actual(Uuid{789}, Uuid{987});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JoinShipMessage, Clone)
{
  JoinShipMessage expected(Uuid{14}, Uuid{17});
  expected.setClientId(Uuid{21});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::JOIN_SHIP);
  assertMessagesAreEqual(cloned->as<JoinShipMessage>(), expected);
}

} // namespace bsgo
