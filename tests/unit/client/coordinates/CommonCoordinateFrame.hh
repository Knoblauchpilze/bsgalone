
#pragma once

#include <CoordinateFrame.hh>
#include <gtest/gtest.h>
#include <olcEngine.hh>
#include <optional>

namespace pge::tests {

struct TestCaseTilesToPixels
{
  std::string name;
  CoordinateFramePtr frame;

  olc::vf2d tiles;
  olc::vf2d expected;
};

using TilesToPixels = ::testing::TestWithParam<TestCaseTilesToPixels>;

auto generateTestNameTilesToPixels(const ::testing::TestParamInfo<TestCaseTilesToPixels> &info)
  -> std::string;

struct TestCasePixelsToTiles
{
  std::string name;
  CoordinateFramePtr frame;

  olc::vf2d pixels;
  olc::vf2d expected;
};

using PixelsToTiles = ::testing::TestWithParam<TestCasePixelsToTiles>;

auto generateTestNamePixelsToTiles(const ::testing::TestParamInfo<TestCasePixelsToTiles> &info)
  -> std::string;

struct TestCasePixelsToTilesIntra
{
  std::string name;
  CoordinateFramePtr frame;

  olc::vf2d pixels;
  olc::vi2d expectedTiles;
  olc::vf2d expectedIntra;
};

using PixelsToTilesIntra = ::testing::TestWithParam<TestCasePixelsToTilesIntra>;

auto generateTestNamePixelsToTilesIntra(
  const ::testing::TestParamInfo<TestCasePixelsToTilesIntra> &info) -> std::string;

namespace constants {
struct Tiles
{
  static const olc::vf2d CENTER;
  static const olc::vf2d DIMS;

  static const olc::vf2d TOP_LEFT;
  static const olc::vf2d TOP_RIGHT;
  static const olc::vf2d BOTTOM_RIGHT;
  static const olc::vf2d BOTTOM_LEFT;
};

struct Pixels
{
  static const olc::vf2d TOP_LEFT;
  static const olc::vf2d DIMS;

  static const olc::vf2d TOP_RIGHT;
  static const olc::vf2d BOTTOM_RIGHT;
  static const olc::vf2d BOTTOM_LEFT;
};
} // namespace constants

} // namespace pge::tests
