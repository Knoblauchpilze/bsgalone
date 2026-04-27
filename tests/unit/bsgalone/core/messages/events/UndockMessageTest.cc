
#include "UndockMessage.hh"
#include "SerializationHelper.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const UndockMessage &actual, const UndockMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Events_UndockMessage, SerializationDeserialization)
{
  UndockMessage expected(Uuid{});

  const auto actual = serializeAndDeserializeMessage(expected);

  assertMessagesAreEqual(actual->as<UndockMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Events_UndockMessage, Clone)
{
  UndockMessage expected(Uuid{});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::UNDOCK);
  assertMessagesAreEqual(cloned->as<UndockMessage>(), expected);
}

} // namespace bsgalone::core
