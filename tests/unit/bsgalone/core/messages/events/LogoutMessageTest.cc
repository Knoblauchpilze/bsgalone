
#include "LogoutMessage.hh"
#include "SerializationHelper.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const LogoutMessage &actual, const LogoutMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Events_LoginMessage, SerializationDeserialization)
{
  LogoutMessage expected;

  const auto actual = serializeAndDeserializeMessage(expected);

  assertMessagesAreEqual(actual->as<LogoutMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Events_LoginMessage, Clone)
{
  LogoutMessage expected;

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOGOUT);
  assertMessagesAreEqual(cloned->as<LogoutMessage>(), expected);
}

} // namespace bsgalone::core
