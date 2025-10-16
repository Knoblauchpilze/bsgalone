
#include "Comparison.hh"
#include "DataSerialization.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {
namespace {
inline bool serializeAndDeserializeData(const ResourceData &value, ResourceData &output)
{
  std::ostringstream out{};
  serializeResourceData(out, value);
  std::istringstream in(out.str());
  return deserializeResourceData(in, output);
}
} // namespace

TEST(Unit_Bsgo_Serialization_ResourceData, EqualWhenDbIdIsEqual)
{
  ResourceData data1{.dbId = Uuid{1234}};

  ResourceData data2{.dbId = Uuid{1234}, .name = "test-resource"};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_ResourceData, DifferentWhenDbIdIsDifferent)
{
  ResourceData data1{.dbId = Uuid{1234}, .name = "resource"};

  ResourceData data2 = data1;
  data2.dbId         = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgo_Serialization_ResourceData, Basic)
{
  ResourceData input{.dbId = Uuid{1234}, .name = "resource1"};

  ResourceData output{.dbId = Uuid{14}, .name = "resource2"};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertResourceDataAreEqual(output, input);
}

TEST(Unit_Bsgo_Serialization_ResourceData, EmptyOutputName)
{
  ResourceData input{.dbId = Uuid{9874}, .name = "resource1"};

  ResourceData output{.dbId = Uuid{35}};

  EXPECT_TRUE(serializeAndDeserializeData(input, output));

  assertResourceDataAreEqual(output, input);
}

} // namespace bsgo
