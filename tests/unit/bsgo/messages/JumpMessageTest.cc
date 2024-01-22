
#include "JumpMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const JumpMessage &actual, const JumpMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.getShipEntityId(), expected.getShipEntityId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_JumpMessage, Basic)
{
  const JumpMessage expected(Uuid{14}, Uuid{26});
  JumpMessage actual(Uuid{6}, Uuid{4});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpMessage, Validated)
{
  JumpMessage expected(Uuid{14}, Uuid{26});
  expected.validate();
  JumpMessage actual(Uuid{6}, Uuid{4});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
