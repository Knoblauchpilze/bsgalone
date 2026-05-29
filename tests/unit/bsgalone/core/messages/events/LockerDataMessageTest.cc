
#include "LockerDataMessage.hh"
#include "SerializationHelper.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const LockerDataMessage &actual, const LockerDataMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Events_LockerDataMessage, SerializationDeserialization)
{
  LockerDataMessage expected(Uuid{});

  const auto actual = serializeAndDeserializeMessage(expected);

  assertMessagesAreEqual(actual->as<LockerDataMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Events_LockerDataMessage, Clone)
{
  LockerDataMessage expected(Uuid{});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::LOCKER_DATA);
  assertMessagesAreEqual(cloned->as<LockerDataMessage>(), expected);
}

} // namespace bsgalone::core
