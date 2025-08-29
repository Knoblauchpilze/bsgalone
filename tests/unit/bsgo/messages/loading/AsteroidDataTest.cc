
#include "Common.hh"
#include "DataSerialization.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
inline bool serializeAndDeserializeData(const AsteroidData &value, AsteroidData &output)
{
  std::ostringstream out{};
  serializeAsteroidData(out, value);
  std::istringstream in(out.str());
  return deserializeAsteroidData(in, output);
}

} // namespace

TEST(Unit_Bsgo_Serialization_AsteroidData, EqualWhenDbIdIsEqual)
{
  AsteroidData data1{.dbId     = Uuid{1234},
                     .position = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                     .radius   = 5.0f,
                     .health   = 100.0f};

  AsteroidData data2{.dbId = Uuid{1234}, .radius = 17.5f};

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

  AsteroidData output{.dbId = Uuid{14}, .resource = Uuid{4567}, .amount = 50};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertAsteroidDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_AsteroidData, WithLoot)
{
  AsteroidData input{.dbId     = Uuid{1234},
                     .position = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                     .radius   = 5.0f,
                     .health   = 100.0f,
                     .resource = Uuid{4567},
                     .amount   = 31};

  AsteroidData output{.dbId = Uuid{14}};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertAsteroidDataAreEqual(output, input);
}

} // namespace bsgo
