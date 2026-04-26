
#include "UndockRequest.hh"
#include "SerializationHelper.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const UndockRequest &actual, const UndockRequest &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Commands_UndockRequest, SerializationDeserialization)
{
  UndockRequest expected(Uuid{});

  const auto actual = serializeAndDeserializeMessage(expected);

  assertMessagesAreEqual(actual->as<UndockRequest>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Commands_UndockRequest, Clone)
{
  UndockRequest expected(Uuid{});

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::UNDOCK_REQUEST);
  assertMessagesAreEqual(cloned->as<UndockRequest>(), expected);
}

} // namespace bsgalone::core
