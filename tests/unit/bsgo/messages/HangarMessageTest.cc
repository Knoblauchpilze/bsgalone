
#include "HangarMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
auto assertMessagesAreEqual(const HangarMessage &actual, const HangarMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_HangarMessage, NotValidated)
{
  const HangarMessage expected(Uuid{14});
  HangarMessage actual(Uuid{6});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_HangarMessage, Validated)
{
  HangarMessage expected(Uuid{14});
  expected.validate();
  HangarMessage actual(Uuid{6});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

} // namespace bsgo
