
#include "LogoutRequest.hh"
#include "SerializationHelper.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const LogoutRequest &actual, const LogoutRequest &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Commands_LogoutRequest, SerializationDeserialization)
{
  LogoutRequest expected(Uuid{18});

  const auto actual = serializeAndDeserializeMessage(expected);

  assertMessagesAreEqual(actual->as<LogoutRequest>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Commands_LogoutRequest, Clone)
{
  LogoutRequest expected(Uuid{18});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOGOUT_REQUEST);
  assertMessagesAreEqual(cloned->as<LogoutRequest>(), expected);
}

} // namespace bsgalone::core
