
#include "Uuid.hh"
#include "Common.hh"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Domain_App_Models_Uuid, GeneratesEmptyUuid)
{
  Uuid uuid{};
  EXPECT_EQ("eee", uuid.str());
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Uuid, DefaultUuidsAreEqual)
{
  Uuid uuid1{};
  Uuid uuid2{};

  EXPECT_EQ(uuid1, uuid2);
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Uuid, RandomUuidsAreNotEqual)
{
  const auto uuid1 = Uuid::random();
  const auto uuid2 = Uuid::random();

  EXPECT_NE(uuid1, uuid2);
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Uuid, CorrectlyParsesValidUuid)
{
  const auto actual = Uuid::fromDbId("0c7a7231-2055-40f6-8f38-b73e821427a4");
  EXPECT_EQ("0c7a7231-2055-40f6-8f38-b73e821427a4", actual.str());
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Uuid, FailsToParseInvalidUuidString)
{
  EXPECT_THAT(Uuid::fromDbId("not-a-uuid"),
              ThrowsMessage<std::invalid_argument>("Unrecognized uuid not-a-uuid"));
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Uuid, toDbId)
{
  const auto uuid = Uuid::fromDbId("0c7a7231-2055-40f6-8f38-b73e821427a4");
  EXPECT_EQ("0c7a7231-2055-40f6-8f38-b73e821427a4", uuid.toDbId());
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Uuid, str)
{
  const auto uuid = Uuid::fromDbId("0c7a7231-2055-40f6-8f38-b73e821427a4");
  EXPECT_EQ("0c7a7231-2055-40f6-8f38-b73e821427a4", uuid.str());
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Uuid, SingleValue)
{
  const Uuid expected = Uuid::random();
  Uuid actual         = Uuid::random();
  serializeAndDeserialize(expected, actual);
  EXPECT_EQ(actual, expected);
}

TEST(Unit_Bsgalone_Core_Domain_App_Models_Uuid, MultipleValues)
{
  Uuid expected1 = Uuid::random();
  Uuid expected2 = Uuid::random();

  std::ostringstream out;
  ::core::serialize(out, expected1);
  ::core::serialize(out, expected2);
  std::istringstream in(out.str());

  Uuid actual1 = Uuid::random();
  Uuid actual2 = Uuid::random();
  ::core::deserialize(in, actual1);
  ::core::deserialize(in, actual2);

  EXPECT_EQ(actual1, expected1);
  EXPECT_EQ(actual2, expected2);
}

} // namespace bsgalone::core
