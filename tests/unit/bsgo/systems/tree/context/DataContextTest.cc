
#include "DataContext.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgo {

TEST(Unit_Bsgo_Systems_DataContext, SetGet)
{
  DataContext ctx;

  ctx.setKey(ContextKey::TARGET_REACHED, Uuid{14});

  const auto actual = ctx.tryGetValue<Uuid>(ContextKey::TARGET_REACHED);

  EXPECT_TRUE(actual.has_value());
  EXPECT_EQ(*actual, Uuid{14});
}

TEST(Unit_Bsgo_Systems_DataContext, HasChangedWhenKeyIsSet)
{
  DataContext ctx;

  ctx.setKey(ContextKey::TARGET_REACHED, Uuid{14});

  EXPECT_TRUE(ctx.changed());
}

TEST(Unit_Bsgo_Systems_DataContext, HasNotChangedWhenMarkedAsSynced)
{
  DataContext ctx;

  ctx.setKey(ContextKey::TARGET_REACHED, Uuid{14});
  ctx.markAsSynced();

  EXPECT_FALSE(ctx.changed());
}

TEST(Unit_Bsgo_Systems_DataContext, HasChangedWhenKeyIsDeleted)
{
  DataContext ctx;

  ctx.setKey(ContextKey::TARGET_REACHED, Uuid{14});
  ctx.markAsSynced();
  EXPECT_FALSE(ctx.changed());

  ctx.clear(ContextKey::TARGET_REACHED);

  EXPECT_TRUE(ctx.changed());
}

TEST(Unit_Bsgo_Systems_DataContext, HasChangedWhenOverridingExistingKey)
{
  DataContext ctx;
  ctx.setKey(ContextKey::TARGET_REACHED, Uuid{14});
  ctx.markAsSynced();

  ctx.setKey(ContextKey::TARGET_REACHED, Uuid{17});

  EXPECT_TRUE(ctx.changed());
}

TEST(Unit_Bsgo_Systems_DataContext, KeyHasChangedWhenOverriding)
{
  DataContext ctx;
  ctx.setKey(ContextKey::TARGET_REACHED, Uuid{14});
  ctx.markAsSynced();

  ctx.setKey(ContextKey::TARGET_REACHED, Uuid{17});

  const auto actual = ctx.tryGetKey(ContextKey::TARGET_REACHED);
  EXPECT_TRUE(actual.has_value());
  EXPECT_TRUE(actual->changed());
}

TEST(Unit_Bsgo_Systems_DataContext, OverridesExistingKey)
{
  DataContext ctx;
  ctx.setKey(ContextKey::TARGET_REACHED, Uuid{14});

  ctx.setKey(ContextKey::TARGET_REACHED, Uuid{17});

  const auto actual = ctx.tryGetValue<Uuid>(ContextKey::TARGET_REACHED);

  EXPECT_TRUE(actual.has_value());
  EXPECT_EQ(*actual, Uuid{17});
}

} // namespace bsgo
