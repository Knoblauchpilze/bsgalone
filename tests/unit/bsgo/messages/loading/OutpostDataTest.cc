
#include "Comparison.hh"
#include "DataSerialization.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
inline bool serializeAndDeserializeData(const OutpostData &value, OutpostData &output)
{
  std::ostringstream out{};
  serializeOutpostData(out, value);
  std::istringstream in(out.str());
  return deserializeOutpostData(in, output);
}
} // namespace

TEST(Unit_Bsgo_Serialization_OutpostData, EqualWhenDbIdIsEqual)
{
  OutpostData data1{.dbId        = Uuid{1234},
                    .position    = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                    .radius      = 5.0f,
                    .powerPoints = 117.42f};

  OutpostData data2{.dbId = Uuid{1234}, .radius = 17.5f};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_OutpostData, DifferentWhenDbIdIsDifferent)
{
  OutpostData data1{.dbId          = Uuid{1234},
                    .position      = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                    .radius        = 5.0f,
                    .hullPoints    = 96.5f,
                    .maxHullPoints = 117.42f};

  OutpostData data2 = data1;
  data2.dbId        = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_OutpostData, Basic)
{
  OutpostData input{.dbId       = Uuid{1234},
                    .position   = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                    .radius     = 5.0f,
                    .hullPoints = 102.0f,
                    .powerRegen = 0.5124f,
                    .faction    = Faction::COLONIAL};

  OutpostData output{.dbId           = Uuid{14},
                     .maxPowerPoints = 25.87f,
                     .faction        = Faction::CYLON,
                     .targetDbId     = Uuid{4567}};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertOutpostDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_OutpostData, WithTarget)
{
  OutpostData input{.dbId       = Uuid{1234},
                    .position   = Eigen::Vector3f{1.0f, 2.0f, 3.0f},
                    .radius     = 5.0f,
                    .hullPoints = 100.0f,
                    .targetDbId = Uuid{4567}};

  OutpostData output{.dbId = Uuid{14}};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertOutpostDataAreEqual(output, input);
}

} // namespace bsgo
