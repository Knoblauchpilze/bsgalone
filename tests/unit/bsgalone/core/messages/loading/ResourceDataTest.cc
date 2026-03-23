
#include "ResourceData.hh"
#include "Common.hh"
#include "Comparison.hh"
#include "SerializationHelper.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Messages_ResourceData, EqualWhenDbIdIsEqual)
{
  ResourceData data1{.dbId = Uuid{1234}};

  ResourceData data2{.dbId = Uuid{1234}, .name = "test-resource"};

  EXPECT_TRUE(data1 == data2);
}

TEST(Unit_Bsgalone_Core_Messages_ResourceData, DifferentWhenDbIdIsDifferent)
{
  ResourceData data1{.dbId = Uuid{1234}, .name = "resource"};

  ResourceData data2 = data1;
  data2.dbId         = Uuid{5678};

  EXPECT_FALSE(data1 == data2);
}

TEST(Unit_Bsgalone_Core_Messages_ResourceData, Basic)
{
  ResourceData input{.dbId = Uuid{1234}, .name = "resource1"};

  ResourceData output{.dbId = Uuid{14}, .name = "resource2"};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertResourceDataAreEqual(output, input);
}

TEST(Unit_Bsgalone_Core_Messages_ResourceData, EmptyOutputName)
{
  ResourceData input{.dbId = Uuid{9874}, .name = "resource1"};

  ResourceData output{.dbId = Uuid{35}};

  EXPECT_TRUE(test::serializeAndDeserialize(input, output));

  assertResourceDataAreEqual(output, input);
}

} // namespace bsgalone::core
