
#include "ScannedMessage.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const ScannedMessage &actual, const ScannedMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getAsteroidDbId(), expected.getAsteroidDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_ScannedMessage, Basic)
{
  const ScannedMessage expected(Uuid{1234}, Uuid{21});
  ScannedMessage actual(Uuid{897}, Uuid{36});
  actual.setClientId(Uuid{78});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ScannedMessage, Validated)
{
  ScannedMessage expected(Uuid{1234}, Uuid{21});
  expected.validate();
  ScannedMessage actual(Uuid{897}, Uuid{36});
  actual.setClientId(Uuid{78});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ScannedMessage, WithClientId)
{
  ScannedMessage expected(Uuid{1234}, Uuid{21});
  expected.setClientId(Uuid{78});
  ScannedMessage actual(Uuid{897}, Uuid{36});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_ScannedMessage, Clone)
{
  const ScannedMessage expected(Uuid{1234}, Uuid{21});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::SCANNED);
  assertMessagesAreEqual(cloned->as<ScannedMessage>(), expected);
}

} // namespace bsgo
