
#include "SystemDataMessage.hh"
#include "SerializationHelper.hh"
#include "TestDataFactory.hh"
#include "TestMessageFactory.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::core {
namespace {
void assertMessagesAreEqual(const SystemDataMessage &actual, const SystemDataMessage &expected)
{
  EXPECT_EQ(actual.type(), expected.type());
  EXPECT_EQ(actual.getPlayerDbId(), expected.getPlayerDbId());
  EXPECT_EQ(actual.getSystemName(), expected.getSystemName());
  EXPECT_EQ(actual.getCurrentTick(), expected.getCurrentTick());
  EXPECT_EQ(actual.getTimeStep(), expected.getTimeStep());
  EXPECT_EQ(actual.getAsteroids(), expected.getAsteroids());
}
} // namespace

TEST(Unit_Bsgalone_Core_Messages_Events_SystemDataMessage, SerializationDeserialization)
{
  std::vector<Asteroid> asteroids{generateAsteroid(), generateAsteroid(true)};
  const auto expected = generateSystemDataMessage(std::move(asteroids));

  const auto actual = serializeAndDeserializeMessage(expected);

  assertMessagesAreEqual(actual->as<SystemDataMessage>(), expected);
}

TEST(Unit_Bsgalone_Core_Messages_Events_SystemDataMessage, Clone)
{
  std::vector<Asteroid> asteroids{generateAsteroid(), generateAsteroid(true)};
  const auto expected = generateSystemDataMessage(std::move(asteroids));

  const auto cloned = expected.clone();

  ASSERT_EQ(cloned->type(), MessageType::SYSTEM_DATA);
  assertMessagesAreEqual(cloned->as<SystemDataMessage>(), expected);
}

} // namespace bsgalone::core
