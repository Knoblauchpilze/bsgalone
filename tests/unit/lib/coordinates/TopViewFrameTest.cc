
#include <gtest/gtest.h>

#include "CommonCoordinateFrame.hh"
#include "TopViewFrame.hh"

using namespace ::testing;

namespace pge::tests {

auto generateTopViewFrame() -> CoordinateFramePtr
{
  CenteredViewport tiles = {constants::Tiles::CENTER, constants::Tiles::DIMS};
  TopLeftViewport pixels = {constants::Pixels::TOP_LEFT, constants::Pixels::DIMS};
  return std::make_shared<TopViewFrame>(tiles, pixels);
}

TEST(Unit_TopViewFrame, Constructor)
{
  auto frame = generateTopViewFrame();

  auto tiles = frame->tilesViewport();
  EXPECT_EQ(tiles.center(), constants::Tiles::CENTER);
  EXPECT_EQ(tiles.dims(), constants::Tiles::DIMS);

  auto tile = frame->tileSize();
  EXPECT_EQ(tile, constants::Pixels::DIMS / constants::Tiles::DIMS);
}

auto generateTopTestCaseTilesToPixels(const std::string &name,
                                      const olc::vf2d &tiles,
                                      const olc::vf2d &expected) -> TestCaseTilesToPixels
{
  return TestCaseTilesToPixels{name, generateTopViewFrame(), tiles, expected};
}

// Useful IEEE calculators:
// http://weitz.de/ieee/
// https://www.h-schmidt.net/FloatConverter/IEEE754.html
INSTANTIATE_TEST_CASE_P(
  Unit_TopViewFrame,
  TilesToPixels,
  Values(generateTopTestCaseTilesToPixels("top_left",
                                          constants::Tiles::TOP_LEFT,
                                          constants::Pixels::TOP_LEFT),
         generateTopTestCaseTilesToPixels("top_right",
                                          constants::Tiles::TOP_RIGHT,
                                          constants::Pixels::TOP_RIGHT),
         generateTopTestCaseTilesToPixels("bottom_right",
                                          constants::Tiles::BOTTOM_RIGHT,
                                          constants::Pixels::BOTTOM_RIGHT),
         generateTopTestCaseTilesToPixels("bottom_left",
                                          constants::Tiles::BOTTOM_LEFT,
                                          constants::Pixels::BOTTOM_LEFT),
         generateTopTestCaseTilesToPixels("inside", {2.5f, -1.0f}, {122.0f, 78.4f}),
         generateTopTestCaseTilesToPixels("x_too_small", {-2.95f, 3.25f}, {-52.4f, 53.75f}),
         generateTopTestCaseTilesToPixels("x_too_large", {12.5f, 3.25f}, {442.0f, 53.75f}),
         generateTopTestCaseTilesToPixels("y_too_small", {-0.75f, -17.2f}, {18.0f, 172.36f}),
         generateTopTestCaseTilesToPixels("y_too_large", {-0.75f, 14.1f}, {18.0f, -9.18f})),
  generateTestNameTilesToPixels);

auto generateTopTestCasePixelsToTiles(const std::string &name,
                                      const olc::vf2d &pixels,
                                      const olc::vf2d &expected) -> TestCasePixelsToTiles
{
  return TestCasePixelsToTiles{name, generateTopViewFrame(), pixels, expected};
}

INSTANTIATE_TEST_CASE_P(
  Unit_TopViewFrame,
  PixelsToTiles,
  Values(generateTopTestCasePixelsToTiles("top_left",
                                          constants::Pixels::TOP_LEFT,
                                          constants::Tiles::TOP_LEFT),
         generateTopTestCasePixelsToTiles("top_right",
                                          constants::Pixels::TOP_RIGHT,
                                          constants::Tiles::TOP_RIGHT),
         generateTopTestCasePixelsToTiles("bottom_right",
                                          constants::Pixels::BOTTOM_RIGHT,
                                          constants::Tiles::BOTTOM_RIGHT),
         generateTopTestCasePixelsToTiles("bottom_left",
                                          constants::Pixels::BOTTOM_LEFT,
                                          constants::Tiles::BOTTOM_LEFT),
         generateTopTestCasePixelsToTiles("inside", {122.0f, 78.4f}, {2.5f, -1.0f}),
         generateTopTestCasePixelsToTiles("x_too_small", {-52.4f, 53.75f}, {-2.95f, 3.25f}),
         generateTopTestCasePixelsToTiles("x_too_large", {442.0f, 53.75f}, {12.5f, 3.25f}),
         generateTopTestCasePixelsToTiles("y_too_small", {18.0f, -9.18f}, {-0.75f, 14.1f}),
         generateTopTestCasePixelsToTiles("y_too_large", {18.0f, 172.36f}, {-0.75f, -17.2f})),
  generateTestNamePixelsToTiles);

auto generateTopTestCasePixelsToTilesIntra(const std::string &name,
                                           const olc::vf2d &pixels,
                                           const olc::vi2d &expectedTiles,
                                           const olc::vf2d &expectedIntra)
  -> TestCasePixelsToTilesIntra
{
  return TestCasePixelsToTilesIntra{name,
                                    generateTopViewFrame(),
                                    pixels,
                                    expectedTiles,
                                    expectedIntra};
}

INSTANTIATE_TEST_CASE_P(
  Unit_TopViewFrame,
  PixelsToTilesIntra,
  Values(generateTopTestCasePixelsToTilesIntra("top_left_quadrant",
                                               olc::vf2d{12.0f, 35.0f},
                                               olc::vi2d{-1, 6},
                                               olc::vf2d{0.0625f, 0.482758522f}),
         generateTopTestCasePixelsToTilesIntra("top_right_quadrant",
                                               olc::vf2d{134.0f, 40.0f},
                                               olc::vi2d{2, 5},
                                               olc::vf2d{0.875f, 0.620689392f}),
         generateTopTestCasePixelsToTilesIntra("bottom_right_quadrant",
                                               olc::vf2d{86.0f, 87.0f},
                                               olc::vi2d{1, -3},
                                               olc::vf2d{0.375f, 0.517241478f}),
         generateTopTestCasePixelsToTilesIntra("bottom_left_quadrant",
                                               olc::vf2d{27.0f, 67.0f},
                                               olc::vi2d{-1, 0},
                                               olc::vf2d{0.53125f, 0.965517282f})),
  generateTestNamePixelsToTilesIntra);

TEST(Unit_TopViewFrame, Translate)
{
  auto frame = generateTopViewFrame();

  olc::vf2d origin{20.0f, 51.0f};
  frame->beginTranslation(origin);

  olc::vf2d translationTiles{2.6f, -1.7f};
  olc::vf2d scale = constants::Pixels::DIMS / constants::Tiles::DIMS;

  auto final = origin + translationTiles * scale;
  frame->translate(final);

  auto tiles = frame->tilesViewport();
  // We moved the position of `[20; 51]` on screen from
  // this position to `origin + translationTiles * scale`.
  // The center will move in the opposite direction to
  // accomodate for `origin` being at its new position
  // on screen. As the y coordinate moves in opposite
  // direction, we have to adjust the translation.
  olc::vf2d centerTranslation{translationTiles.x, -translationTiles.y};
  auto finalTiles = constants::Tiles::CENTER - centerTranslation;
  EXPECT_EQ(tiles.center(), finalTiles);
}

TEST(Unit_TopViewFrame, Translate_PreserveTileSize)
{
  auto frame = generateTopViewFrame();

  auto tile = frame->tileSize();
  EXPECT_EQ(tile, constants::Pixels::DIMS / constants::Tiles::DIMS);

  olc::vf2d origin{20.0f, 51.0f};
  frame->beginTranslation(origin);

  olc::vf2d final{37.2f, 43.2f};
  frame->translate(final);

  tile = frame->tileSize();
  EXPECT_EQ(tile, constants::Pixels::DIMS / constants::Tiles::DIMS);
}

TEST(Unit_TopViewFrame, ZoomIn)
{
  auto frame = generateTopViewFrame();

  olc::vf2d zoomCenter{35.0f, 56.0f};
  auto tilesPos = frame->pixelsToTiles(zoomCenter.x, zoomCenter.y);

  frame->zoomIn(zoomCenter);

  auto newTilesPos = frame->pixelsToTiles(zoomCenter.x, zoomCenter.y);
  EXPECT_EQ(newTilesPos, tilesPos);
}

TEST(Unit_TopViewFrame, ZoomIn_DoubleTileDimensions)
{
  auto frame = generateTopViewFrame();

  olc::vf2d zoomCenter{35.0f, 56.0f};
  frame->zoomIn(zoomCenter);

  auto tile = frame->tileSize();
  EXPECT_EQ(tile, (constants::Pixels::DIMS / constants::Tiles::DIMS) * 2.0f);
}

TEST(Unit_TopViewFrame, ZoomIn_HalveTilesViewport)
{
  auto frame = generateTopViewFrame();

  olc::vf2d zoomCenter{35.0f, 56.0f};
  frame->zoomIn(zoomCenter);

  auto dims = frame->tilesViewport().dims();
  EXPECT_EQ(dims, constants::Tiles::DIMS / 2.0f);
}

TEST(Unit_TopViewFrame, ZoomOut)
{
  auto frame = generateTopViewFrame();

  olc::vf2d zoomCenter{118.0f, 72.0f};
  auto tilesPos = frame->pixelsToTiles(zoomCenter.x, zoomCenter.y);

  frame->zoomOut(zoomCenter);

  auto newTilesPos = frame->pixelsToTiles(zoomCenter.x, zoomCenter.y);
  EXPECT_EQ(newTilesPos, tilesPos);
}

TEST(Unit_TopViewFrame, ZoomOut_HalveTileDimensions)
{
  auto frame = generateTopViewFrame();

  olc::vf2d zoomCenter{35.0f, 56.0f};
  frame->zoomOut(zoomCenter);

  auto tile = frame->tileSize();
  EXPECT_EQ(tile, (constants::Pixels::DIMS / constants::Tiles::DIMS) / 2.0f);
}

TEST(Unit_TopViewFrame, ZoomOut_DoubleTilesViewport)
{
  auto frame = generateTopViewFrame();

  olc::vf2d zoomCenter{35.0f, 56.0f};
  frame->zoomOut(zoomCenter);

  auto dims = frame->tilesViewport().dims();
  EXPECT_EQ(dims, constants::Tiles::DIMS * 2.0f);
}

} // namespace pge::tests
