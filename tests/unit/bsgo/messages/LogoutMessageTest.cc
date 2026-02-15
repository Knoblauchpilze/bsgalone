
#include "LogoutMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const LogoutMessage &actual, const LogoutMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.shouldCloseConnection(), expected.shouldCloseConnection());
  EXPECT_EQ(actual.tryGetSystemDbId(), expected.tryGetSystemDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_LogoutMessage, CorrectlyUpdatesCloseConnection)
{
  const LogoutMessage expected(Uuid{36}, true);

  LogoutMessage actual(Uuid{77451}, false);
  actual.setClientId(Uuid{26});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LogoutMessage, ClearsSystemDbId)
{
  LogoutMessage expected(Uuid{36}, true);

  LogoutMessage actual(Uuid{77451});
  actual.setClientId(Uuid{26});
  actual.setSystemDbId(Uuid{71});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LogoutMessage, OverridesSystemDbId)
{
  LogoutMessage expected(Uuid{36}, true);
  expected.setSystemDbId(Uuid{71});

  LogoutMessage actual(Uuid{77451});
  actual.setClientId(Uuid{26});
  actual.setSystemDbId(Uuid{98});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LogoutMessage, WithClientId)
{
  LogoutMessage expected(Uuid{101202}, true);
  expected.setClientId(Uuid{119});

  LogoutMessage actual(Uuid{14});

  serializeAndDeserializeMessage(expected, actual);

  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LogoutMessage, Clone)
{
  LogoutMessage expected(Uuid{101202}, true);
  expected.setSystemDbId(Uuid{17});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), bsgalone::core::MessageType::LOGOUT);
  assertMessagesAreEqual(cloned->as<LogoutMessage>(), expected);
}

} // namespace bsgo
