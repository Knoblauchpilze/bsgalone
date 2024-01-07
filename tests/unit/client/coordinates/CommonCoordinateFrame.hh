
#pragma once

#include <CoordinateFrame.hh>
#include <gtest/gtest.h>
#include <optional>

namespace pge::tests {

struct TestCaseTilesToPixels
{
  std::string name;
  CoordinateFramePtr frame;

  Vec2f tiles;
  Vec2f expected;
};

using TilesToPixels = ::testing::TestWithParam<TestCaseTilesToPixels>;

auto generateTestNameTilesToPixels(const ::testing::TestParamInfo<TestCaseTilesToPixels> &info)
  -> std::string;

struct TestCasePixelsToTiles
{
  std::string name;
  CoordinateFramePtr frame;

  Vec2f pixels;
  Vec2f expected;
};

using PixelsToTiles = ::testing::TestWithParam<TestCasePixelsToTiles>;

auto generateTestNamePixelsToTiles(const ::testing::TestParamInfo<TestCasePixelsToTiles> &info)
  -> std::string;

struct TestCasePixelsToTilesIntra
{
  std::string name;
  CoordinateFramePtr frame;

  Vec2f pixels;
  Vec2i expectedTiles;
  Vec2f expectedIntra;
};

using PixelsToTilesIntra = ::testing::TestWithParam<TestCasePixelsToTilesIntra>;

auto generateTestNamePixelsToTilesIntra(
  const ::testing::TestParamInfo<TestCasePixelsToTilesIntra> &info) -> std::string;

namespace constants {
struct Tiles
{
  static const Vec2f CENTER;
  static const Vec2f DIMS;

  static const Vec2f TOP_LEFT;
  static const Vec2f TOP_RIGHT;
  static const Vec2f BOTTOM_RIGHT;
  static const Vec2f BOTTOM_LEFT;
};

struct Pixels
{
  static const Vec2f TOP_LEFT;
  static const Vec2f DIMS;

  static const Vec2f TOP_RIGHT;
  static const Vec2f BOTTOM_RIGHT;
  static const Vec2f BOTTOM_LEFT;
};
} // namespace constants

} // namespace pge::tests
