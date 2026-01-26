
#include "CommonCoordinateFrame.hh"
#include <gtest/gtest.h>

using namespace ::testing;

namespace pge::tests {
namespace constants {
const Vec2f Tiles::CENTER = {1.0f, 2.0f};
const Vec2f Tiles::DIMS   = {4.0f, 10.0f};

const Vec2f Tiles::TOP_LEFT     = {Tiles::CENTER.x - Tiles::DIMS.x / 2.0f,
                                   Tiles::CENTER.y + Tiles::DIMS.y / 2.0f};
const Vec2f Tiles::TOP_RIGHT    = {Tiles::CENTER.x + Tiles::DIMS.x / 2.0f,
                                   Tiles::CENTER.y + Tiles::DIMS.y / 2.0f};
const Vec2f Tiles::BOTTOM_RIGHT = {Tiles::CENTER.x + Tiles::DIMS.x / 2.0f,
                                   Tiles::CENTER.y - Tiles::DIMS.y / 2.0f};
const Vec2f Tiles::BOTTOM_LEFT  = {Tiles::CENTER.x - Tiles::DIMS.x / 2.0f,
                                   Tiles::CENTER.y - Tiles::DIMS.y / 2.0f};

const Vec2f Pixels::TOP_LEFT = {10.0f, 32.0f};
const Vec2f Pixels::DIMS     = {128.0f, 58.0f};

const Vec2f Pixels::TOP_RIGHT{TOP_LEFT.x + DIMS.x, TOP_LEFT.y};
const Vec2f Pixels::BOTTOM_RIGHT{TOP_LEFT + DIMS};
const Vec2f Pixels::BOTTOM_LEFT{TOP_LEFT.x, TOP_LEFT.y + DIMS.y};
} // namespace constants

TEST_P(TilesToPixels, Test)
{
  const auto param = GetParam();

  const auto pixels = param.frame->tilesToPixels(param.tiles.x, param.tiles.y);

  EXPECT_EQ(param.expected, pixels);
}

auto generateTestNameTilesToPixels(const ::testing::TestParamInfo<TestCaseTilesToPixels> &info)
  -> std::string
{
  return info.param.name;
}

TEST_P(PixelsToTiles, Test)
{
  const auto param = GetParam();

  const auto tiles = param.frame->pixelsToTiles(param.pixels.x, param.pixels.y);

  EXPECT_EQ(param.expected, tiles);
}

auto generateTestNamePixelsToTiles(const ::testing::TestParamInfo<TestCasePixelsToTiles> &info)
  -> std::string
{
  return info.param.name;
}

TEST_P(PixelsToTilesIntra, Test)
{
  const auto param = GetParam();

  Vec2f intra;
  const auto tiles = param.frame->pixelsToTilesAndIntra(param.pixels, &intra);

  EXPECT_EQ(param.expectedTiles, tiles);
  EXPECT_EQ(param.expectedIntra, intra);
}

auto generateTestNamePixelsToTilesIntra(
  const ::testing::TestParamInfo<TestCasePixelsToTilesIntra> &info) -> std::string
{
  return info.param.name;
}

} // namespace pge::tests
