
#include <gtest/gtest.h>

#include "CommonCoordinateFrame.hh"
#include "IsometricViewFrame.hh"

using namespace ::testing;

namespace pge::tests {

auto generateIsometricViewFrame() -> CoordinateFramePtr
{
  CenteredViewport tiles = {constants::Tiles::CENTER, constants::Tiles::DIMS};
  TopLeftViewport pixels = {constants::Pixels::TOP_LEFT, constants::Pixels::DIMS};
  return std::make_shared<IsometricViewFrame>(tiles, pixels);
}

TEST(Unit_IsometricViewFrame, Constructor)
{
  auto frame = generateIsometricViewFrame();

  auto tiles = frame->tilesViewport();
  EXPECT_EQ(tiles.center(), constants::Tiles::CENTER);
  EXPECT_EQ(tiles.dims(), constants::Tiles::DIMS);

  auto tile = frame->tileSize();
  EXPECT_EQ(tile, constants::Pixels::DIMS / constants::Tiles::DIMS);
}

auto generateIsometricTestCaseTilesToPixels(const std::string &name,
                                            const olc::vf2d &tiles,
                                            const olc::vf2d &expected) -> TestCaseTilesToPixels
{
  return TestCaseTilesToPixels{name, generateIsometricViewFrame(), tiles, expected};
}

INSTANTIATE_TEST_CASE_P(
  Unit_IsometricViewFrame,
  TilesToPixels,
  Values(generateIsometricTestCaseTilesToPixels(
           "top_left",
           constants::Tiles::TOP_LEFT,
           {constants::Pixels::TOP_LEFT.x + constants::Pixels::DIMS.x / 2.0f, 19.987808f}),
         generateIsometricTestCaseTilesToPixels(
           "top_right",
           constants::Tiles::TOP_RIGHT,
           {164.509674f, constants::Pixels::TOP_LEFT.y + constants::Pixels::DIMS.y / 2.0f}),
         generateIsometricTestCaseTilesToPixels(
           "bottom_right",
           constants::Tiles::BOTTOM_RIGHT,
           {constants::Pixels::TOP_LEFT.x + constants::Pixels::DIMS.x / 2.0f, 102.012199f}),
         generateIsometricTestCaseTilesToPixels(
           "bottom_left",
           constants::Tiles::BOTTOM_LEFT,
           {-16.509666f, constants::Pixels::TOP_LEFT.y + constants::Pixels::DIMS.y / 2.0f}),
         generateIsometricTestCaseTilesToPixels("inside", {2.5f, -1.0f}, {80.788223f, 88.683228f}),
         generateIsometricTestCaseTilesToPixels("x_too_small",
                                                {-2.95f, 3.25f},
                                                {-4.06459045f, 15.373938f}),
         generateIsometricTestCaseTilesToPixels("x_too_large",
                                                {12.5f, 3.25f},
                                                {345.529022f, 173.783539f}),
         generateIsometricTestCaseTilesToPixels("y_too_small",
                                                {-0.75f, -17.2f},
                                                {-139.376541f, 121.800575f}),
         generateIsometricTestCaseTilesToPixels("y_too_large",
                                                {-0.75f, 14.1f},
                                                {143.918716f, -6.56759262f})),
  generateTestNameTilesToPixels);

auto generateIsometricTestCasePixelsToTiles(const std::string &name,
                                            const olc::vf2d &pixels,
                                            const olc::vf2d &expected) -> TestCasePixelsToTiles
{
  return TestCasePixelsToTiles{name, generateIsometricViewFrame(), pixels, expected};
}

INSTANTIATE_TEST_CASE_P(
  Unit_IsometricViewFrame,
  PixelsToTiles,
  Values(
    generateIsometricTestCasePixelsToTiles("top_left",
                                           constants::Pixels::TOP_LEFT,
                                           {-1.82842731f, constants::Tiles::CENTER.y}),
    generateIsometricTestCasePixelsToTiles("top_right",
                                           constants::Pixels::TOP_RIGHT,
                                           {constants::Tiles::CENTER.x,
                                            constants::Tiles::CENTER.y
                                              + std::sqrt(2.0f) * constants::Tiles::DIMS.y / 2.0f}),
    generateIsometricTestCasePixelsToTiles("bottom_right",
                                           constants::Pixels::BOTTOM_RIGHT,
                                           {3.82842731f, constants::Tiles::CENTER.y}),
    generateIsometricTestCasePixelsToTiles("bottom_left",
                                           constants::Pixels::BOTTOM_LEFT,
                                           {constants::Tiles::CENTER.x, -5.07106829f}),
    generateIsometricTestCasePixelsToTiles("inside", {122.0f, 78.4f}, {2.90918851f, 2.53033018f}),
    generateIsometricTestCasePixelsToTiles("x_too_small",
                                           {-52.4f, 53.75f},
                                           {-2.14662552f, -4.09879589f}),
    generateIsometricTestCasePixelsToTiles("x_too_large",
                                           {442.0f, 53.75f},
                                           {8.77817535f, 23.2132053f}),
    generateIsometricTestCasePixelsToTiles("y_too_small",
                                           {18.0f, 172.36f},
                                           {5.19314384f, -14.6700439f}),
    generateIsometricTestCasePixelsToTiles("y_too_large",
                                           {18.0f, -9.18f},
                                           {-3.65983391f, 7.46240044f})),
  generateTestNamePixelsToTiles);

auto generateIsometricTestCasePixelsToTilesIntra(const std::string &name,
                                                 const olc::vf2d &pixels,
                                                 const olc::vi2d &expectedTiles,
                                                 const olc::vf2d &expectedIntra)
  -> TestCasePixelsToTilesIntra
{
  return TestCasePixelsToTilesIntra{name,
                                    generateIsometricViewFrame(),
                                    pixels,
                                    expectedTiles,
                                    expectedIntra};
}

INSTANTIATE_TEST_CASE_P(
  Unit_IsometricViewFrame,
  PixelsToTilesIntra,
  Values(generateIsometricTestCasePixelsToTilesIntra("top_left_quadrant",
                                                     olc::vf2d{67.904251f, 21.468311f},
                                                     olc::vi2d{-2, 6},
                                                     olc::vf2d{0.937499762f, 0.482758999f}),
         generateIsometricTestCasePixelsToTilesIntra("top_right_quadrant",
                                                     olc::vf2d{149.197144f, 65.375229f},
                                                     olc::vi2d{2, 5},
                                                     olc::vf2d{0.875000238f, 0.620688915f}),
         generateIsometricTestCasePixelsToTilesIntra("bottom_right_quadrant",
                                                     olc::vf2d{32.548912f, 87.472305f},
                                                     olc::vi2d{1, -4},
                                                     olc::vf2d{0.374999762f, 0.482758522f}),
         generateIsometricTestCasePixelsToTilesIntra("bottom_left_quadrant",
                                                     olc::vf2d{7.361282f, 39.289612f},
                                                     olc::vi2d{-2, 0},
                                                     olc::vf2d{0.468749762f, 0.965517282f})),
  generateTestNamePixelsToTilesIntra);

TEST(Unit_IsometricViewFrame, Translate)
{
  auto frame = generateIsometricViewFrame();

  olc::vf2d origin{20.0f, 51.0f};
  frame->beginTranslation(origin);

  olc::vf2d final{110.0f, 75.0f};
  frame->translate(final);

  auto tiles = frame->tilesViewport();
  olc::vf2d finalTiles{-2.15912175f, -0.0458850861f};
  EXPECT_EQ(tiles.center(), finalTiles);
}

TEST(Unit_IsometricViewFrame, Translate_PreserveTileSize)
{
  auto frame = generateIsometricViewFrame();

  auto tile = frame->tileSize();
  EXPECT_EQ(tile, constants::Pixels::DIMS / constants::Tiles::DIMS);

  olc::vf2d origin{20.0f, 51.0f};
  frame->beginTranslation(origin);

  olc::vf2d final{37.2f, 43.2f};
  frame->translate(final);

  tile = frame->tileSize();
  EXPECT_EQ(tile, constants::Pixels::DIMS / constants::Tiles::DIMS);
}

TEST(Unit_IsometricViewFrame, ZoomIn)
{
  auto frame = generateIsometricViewFrame();

  olc::vf2d zoomCenter{35.0f, 56.0f};
  auto tilesPos = frame->pixelsToTiles(zoomCenter.x, zoomCenter.y);

  frame->zoomIn(zoomCenter);

  auto newTilesPos = frame->pixelsToTiles(zoomCenter.x, zoomCenter.y);
  EXPECT_EQ(newTilesPos, tilesPos);
}

TEST(Unit_IsometricViewFrame, ZoomIn_DoubleTileDimensions)
{
  auto frame = generateIsometricViewFrame();

  olc::vf2d zoomCenter{35.0f, 56.0f};
  frame->zoomIn(zoomCenter);

  auto tile = frame->tileSize();
  EXPECT_EQ(tile, (constants::Pixels::DIMS / constants::Tiles::DIMS) * 2.0f);
}

TEST(Unit_IsometricViewFrame, ZoomIn_HalveTilesViewport)
{
  auto frame = generateIsometricViewFrame();

  olc::vf2d zoomCenter{35.0f, 56.0f};
  frame->zoomIn(zoomCenter);

  auto dims = frame->tilesViewport().dims();
  EXPECT_EQ(dims, constants::Tiles::DIMS / 2.0f);
}

TEST(Unit_IsometricViewFrame, ZoomOut)
{
  auto frame = generateIsometricViewFrame();

  olc::vf2d zoomCenter{118.0f, 72.0f};
  auto tilesPos = frame->pixelsToTiles(zoomCenter.x, zoomCenter.y);

  frame->zoomOut(zoomCenter);

  auto newTilesPos = frame->pixelsToTiles(zoomCenter.x, zoomCenter.y);
  EXPECT_EQ(newTilesPos, tilesPos);
}

TEST(Unit_IsometricViewFrame, ZoomOut_HalveTileDimensions)
{
  auto frame = generateIsometricViewFrame();

  olc::vf2d zoomCenter{35.0f, 56.0f};
  frame->zoomOut(zoomCenter);

  auto tile = frame->tileSize();
  EXPECT_EQ(tile, (constants::Pixels::DIMS / constants::Tiles::DIMS) / 2.0f);
}

TEST(Unit_IsometricViewFrame, ZoomOut_DoubleTilesViewport)
{
  auto frame = generateIsometricViewFrame();

  olc::vf2d zoomCenter{35.0f, 56.0f};
  frame->zoomOut(zoomCenter);

  auto dims = frame->tilesViewport().dims();
  EXPECT_EQ(dims, constants::Tiles::DIMS * 2.0f);
}

} // namespace pge::tests
