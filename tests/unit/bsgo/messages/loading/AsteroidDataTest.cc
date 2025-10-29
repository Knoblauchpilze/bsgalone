
#include "AsteroidData.hh"
#include "Common.hh"
#include "Comparison.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {

TEST(Unit_Bsgo_Serialization_AsteroidData, EqualWhenDbIdIsEqual)
{
  AsteroidData data1{.dbId     = Uuid{1234},
                     .position = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                     .radius   = 5.0f,
                     .health   = 100.0f};

  AsteroidData data2{.dbId = Uuid{1234}, .radius = 17.5f, .maxHealth = 14.58f};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_AsteroidData, DifferentWhenDbIdIsDifferent)
{
  AsteroidData data1{.dbId     = Uuid{1234},
                     .position = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                     .radius   = 5.0f,
                     .health   = 100.0f};

  AsteroidData data2 = data1;
  data2.dbId         = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_AsteroidData, Basic)
{
  AsteroidData input{.dbId     = Uuid{1234},
                     .position = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                     .radius   = 5.0f,
                     .health   = 100.0f};

  AsteroidData output{.dbId = Uuid{14}, .maxHealth = 45.78f, .resource = Uuid{4567}, .amount = 50};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertAsteroidDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_AsteroidData, WithLoot)
{
  AsteroidData input{.dbId      = Uuid{1234},
                     .position  = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                     .radius    = 5.0f,
                     .health    = 100.0f,
                     .maxHealth = 1.0235f,
                     .resource  = Uuid{4567},
                     .amount    = 31};

  AsteroidData output{.dbId = Uuid{14}};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertAsteroidDataAreEqual(output, input);
}

} // namespace bsgo
