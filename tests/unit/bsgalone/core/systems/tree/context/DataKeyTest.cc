
#include "DataKey.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Systems_Tree_Context_DataKey, IsMarkedAsChangedUponCreation)
{
  const auto key = DataKey::fromValue(Uuid{19});

  EXPECT_TRUE(key.changed());
}

} // namespace bsgalone::core
