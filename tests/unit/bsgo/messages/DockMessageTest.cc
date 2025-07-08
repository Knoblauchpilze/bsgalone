
#include "DockMessage.hh"
#include "Common.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
void assertMessagesAreEqual(const DockMessage &actual, const DockMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getShipDbId(), expected.getShipDbId());
  EXPECT_EQ(actual.isDocking(), expected.isDocking());
  EXPECT_EQ(actual.getSystemDbId(), expected.getSystemDbId());
  EXPECT_EQ(actual.tryGetClientId(), expected.tryGetClientId());
  EXPECT_EQ(actual.validated(), expected.validated());
}
} // namespace

TEST(Unit_Bsgo_Serialization_DockMessage, Docking)
{
  const DockMessage expected(Uuid{44}, true, Uuid{123456});
  DockMessage actual(Uuid{26}, false, Uuid{17});
  actual.setClientId(Uuid{89765});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, Docking_Validated)
{
  DockMessage expected(Uuid{44}, true, Uuid{123456});
  expected.validate();
  DockMessage actual(Uuid{26}, false, Uuid{17});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, NotDocking)
{
  const DockMessage expected(Uuid{44}, false, Uuid{123456});
  DockMessage actual(Uuid{26}, true, Uuid{17});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, NotDocking_Validated)
{
  DockMessage expected(Uuid{44}, false, Uuid{123456});
  expected.validate();
  DockMessage actual(Uuid{26}, true, Uuid{17});
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, WithClientId)
{
  DockMessage expected(Uuid{44}, false, Uuid{123456});
  expected.setClientId(Uuid{56789});
  DockMessage actual(Uuid{26}, true, Uuid{17});
  actual.validate();
  serializeAndDeserializeMessage(expected, actual);
  assertMessagesAreEqual(actual, expected);
}

TEST(Unit_Bsgo_Serialization_DockMessage, Clone)
{
  DockMessage expected(Uuid{44}, false, Uuid{123456});
  expected.setClientId(Uuid{56789});
  const auto cloned = expected.clone();
  ASSERT_EQ(cloned->type(), MessageType::DOCK);
  assertMessagesAreEqual(cloned->as<DockMessage>(), expected);
}

} // namespace bsgo
