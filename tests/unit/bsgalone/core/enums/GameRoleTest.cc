
#include "GameRole.hh"
#include <gtest/gtest.h>

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Enums_GameRole, Str)
{
  auto actual = str(GameRole::PILOT);
  EXPECT_EQ("pilot", actual);

  actual = str(GameRole::GUNNER);
  EXPECT_EQ("gunner", actual);
}

TEST(Unit_Bsgalone_Core_Enums_GameRole, ToDbGameRole)
{
  auto actual = toDbGameRole(GameRole::PILOT);
  EXPECT_EQ("pilot", actual);

  actual = toDbGameRole(GameRole::GUNNER);
  EXPECT_EQ("gunner", actual);
}

TEST(Unit_Bsgalone_Core_Enums_GameRole, FromDbGameRole)
{
  auto actual = fromDbGameRole("pilot");
  EXPECT_EQ(GameRole::PILOT, actual);

  actual = fromDbGameRole("gunner");
  EXPECT_EQ(GameRole::GUNNER, actual);
}

TEST(Unit_Bsgalone_Core_Enums_GameRole, FromDbGameRole_FailsWhenValueIsNotRecognized)
{
  EXPECT_THROW([] { fromDbGameRole("not-a-role"); }(), std::invalid_argument);
}

} // namespace bsgalone::core
