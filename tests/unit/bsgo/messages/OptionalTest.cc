
#include "Common.hh"
#include "Faction.hh"
#include "Slot.hh"
#include "Uuid.hh"
#include <gtest/gtest.h>
#include <optional>

using namespace ::testing;
using namespace test;

namespace bsgo {
TEST(Unit_Bsgo_Serialization_Optional_Uuid, Empty)
{
  const std::optional<Uuid> expected{};
  std::optional<Uuid> actual{Uuid{1}};
  serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgo_Serialization_Optional_Uuid, WithValue)
{
  const std::optional<Uuid> expected{Uuid{14}};
  std::optional<Uuid> actual{};
  serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgo_Serialization_Optional_Uuid, EmptyAndSomeValueAfter)
{
  const std::optional<Uuid> expectedOpt{};
  const auto expectedFloat{1.68f};

  std::ostringstream out;
  core::serialize(out, expectedOpt);
  core::serialize(out, expectedFloat);
  std::istringstream in(out.str());

  std::optional<Uuid> actualOpt{};
  float actualFloat{};
  core::deserialize(in, actualOpt);
  core::deserialize(in, actualFloat);

  EXPECT_EQ(actualOpt, expectedOpt);
  EXPECT_EQ(actualFloat, expectedFloat);
}

TEST(Unit_Bsgo_Serialization_Optional_Uuid, WithValueAndSomeValueAfter)
{
  const std::optional<Uuid> expectedOpt{};
  const auto expectedSlot{bsgalone::core::Slot::COMPUTER};

  std::ostringstream out;
  core::serialize(out, expectedOpt);
  core::serialize(out, expectedSlot);
  std::istringstream in(out.str());

  std::optional<Uuid> actualOpt{};
  bsgalone::core::Slot actualSlot{};
  core::deserialize(in, actualOpt);
  core::deserialize(in, actualSlot);

  EXPECT_EQ(actualOpt, expectedOpt);
  EXPECT_EQ(actualSlot, expectedSlot);
}

TEST(Unit_Bsgo_Serialization_Optional_Enum, Empty)
{
  const std::optional<Faction> expected{};
  std::optional<Faction> actual{Faction::COLONIAL};
  serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgo_Serialization_Optional_Enum, WithValue)
{
  const std::optional<Faction> expected{Faction::CYLON};
  std::optional<Faction> actual{};
  serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

} // namespace bsgo
