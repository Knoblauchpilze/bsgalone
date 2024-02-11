
#include "LogoutMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const LogoutMessage &actual, const LogoutMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.shouldCloseConnection(), expected.shouldCloseConnection());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_LogoutMessage, PlayerId)
{
  const LogoutMessage expected(Uuid{36});
  LogoutMessage actual(Uuid{77451}, true);
  actual.setClientId(Uuid{26});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LogoutMessage, PlayerAndCloseConnection)
{
  const LogoutMessage expected(Uuid{36}, true);
  LogoutMessage actual(Uuid{77451});
  actual.setClientId(Uuid{26});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_LogoutMessage, Validated)
{
  LogoutMessage expected(Uuid{36});
  expected.validate();
  LogoutMessage actual(Uuid{77451});
  actual.setClientId(Uuid{26});
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
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::LOGOUT);
  assertMessagesAreEqual(cloned->as<LogoutMessage>(), expected);
}

} // namespace bsgo
