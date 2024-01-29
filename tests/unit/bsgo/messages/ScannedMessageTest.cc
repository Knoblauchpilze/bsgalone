
#include "ScannedMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const ScannedMessage &actual, const ScannedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.asteroidEntityId(), expected.asteroidEntityId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
}
} // namespace

TEST(Unit_Bsgo_Serialization_ScannedMessage, Basic)
{
  const ScannedMessage expected(Uuid{21});
  ScannedMessage actual(Uuid{36});
  actual.setClientId(Uuid{78});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ScannedMessage, WithClientId)
{
  ScannedMessage expected(Uuid{21});
  expected.setClientId(Uuid{78});
  ScannedMessage actual(Uuid{36});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ScannedMessage, Clone)
{
  const ScannedMessage expected(Uuid{21});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::SCANNED);
  assertMessagesAreEqual(cloned->as<ScannedMessage>(), expected);
}

} // namespace bsgo
