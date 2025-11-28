
#include "DataKey.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {

TEST(Unit_Bsgo_Systems_DataKey, IsMarkedAsChangedUponCreation)
{
  const auto key = DataKey::fromValue(Uuid{19});

  EXPECT_TRUE(key.changed());
}

} // namespace bsgo
