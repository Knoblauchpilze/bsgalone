
#include "Common.hh"
#include "DataSerialization.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
inline bool serializeAndDeserializeData(const SystemData &value, SystemData &output)
{
  std::ostringstream out{};
  serializeSystemData(out, value);
  std::istringstream in(out.str());
  return deserializeSystemData(in, output);
}
} // namespace

TEST(Unit_Bsgo_Serialization_SystemData, EqualWhenDbIdIsEqual)
{
  SystemData data1{.dbId     = Uuid{1234},
                   .name     = "system-1",
                   .position = Eigen::Vector3f{1.0f, 2.0, 3.0f}};

  SystemData data2{.dbId = Uuid{1234}, .name = "system-2", .position = Eigen::Vector3f::Ones(3)};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_SystemData, DifferentWhenDbIdIsDifferent)
{
  SystemData data1{.dbId     = Uuid{1234},
                   .name     = "a-place",
                   .position = Eigen::Vector3f{3.0f, 2.0f, -1.5f}};

  SystemData data2 = data1;
  data2.dbId       = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_SystemData, Basic)
{
  SystemData input{.dbId     = Uuid{1234},
                   .name     = "test-system",
                   .position = Eigen::Vector3f{1.2f, 4.3f, -5.7f}};

  SystemData output{.dbId     = Uuid{14},
                    .name     = "another-system",
                    .position = Eigen::Vector3f(-3.5f, -2.7f, 1678.654f)};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertSystemDataAreEqual(output, input);
}

} // namespace bsgo
