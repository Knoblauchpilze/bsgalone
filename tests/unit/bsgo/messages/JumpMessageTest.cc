
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
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_JumpMessage, Basic)
{
  const JumpMessage expected(Uuid{14});
  JumpMessage actual(Uuid{6});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpMessage, Validated)
{
  JumpMessage expected(Uuid{14});
  expected.validate();
  JumpMessage actual(Uuid{6});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpMessage, WithClientId)
{
  JumpMessage expected(Uuid{14});
  expected.setClientId(Uuid{119});
  JumpMessage actual(Uuid{6});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_JumpMessage, Clone)
{
  const JumpMessage expected(Uuid{14});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::JUMP);
  assertMessagesAreEqual(cloned->as<JumpMessage>(), expected);
}

} // namespace bsgo
