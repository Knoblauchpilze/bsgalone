
#include "Controls.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace pge::controls {

TEST(Controls, Test_Constructor)
{
  auto state = newState();

  EXPECT_EQ(state.mPosX, 0);
  EXPECT_EQ(state.mPosY, 0);

  EXPECT_FALSE(state.tab);

  EXPECT_EQ(state.keys.size(), keys::KEYS_COUNT);
  EXPECT_EQ(state.buttons.size(), mouse::BUTTONS_COUNT);

  for (const auto &key : state.keys)
  {
    EXPECT_FALSE(key);
  }

  for (const auto &button : state.buttons)
  {
    EXPECT_EQ(ButtonState::FREE, button);
  }
}

} // namespace pge::controls
