
#include "HangarDataMessage.hh"
#include "SerializationHelper.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const HangarDataMessage &actual, const HangarDataMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Events_HangarDataMessage, SerializationDeserialization)
{
  HangarDataMessage expected(Uuid{});

  const auto actual = serializeAndDeserializeMessage(expected);

  assertMessagesAreEqual(actual->as<HangarDataMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Events_HangarDataMessage, Clone)
{
  HangarDataMessage expected(Uuid{});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::HANGAR_DATA);
  assertMessagesAreEqual(cloned->as<HangarDataMessage>(), expected);
}

} // namespace bsgalone::core
