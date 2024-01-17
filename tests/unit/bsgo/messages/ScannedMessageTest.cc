
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
}
} // namespace

TEST(Unit_Bsgo_Serialization_ScannedMessage, Basic)
{
  const ScannedMessage expected(Uuid{21});
  ScannedMessage actual(Uuid{36});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
