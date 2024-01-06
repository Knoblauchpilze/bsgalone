
#pragma once

#include "UiMenu.hh"
#include <optional>
#include <string>

namespace pge {

auto generateBlankHorizontalMenu(const std::optional<olc::vi2d> &dims = {}) -> UiMenuPtr;
auto generateBlankHorizontalMenu(const olc::vi2d &pos, const std::optional<olc::vi2d> &dims = {})
  -> UiMenuPtr;
auto generateBlankVerticalMenu(const std::optional<olc::vi2d> &dims = {}) -> UiMenuPtr;
auto generateBlankVerticalMenu(const olc::vi2d &pos, const std::optional<olc::vi2d> &dims = {})
  -> UiMenuPtr;
auto generateBlankMenu(const olc::vi2d &pos,
                       const MenuLayout &layout,
                       const std::optional<olc::vi2d> &dims = {}) -> UiMenuPtr;
auto generateBlankMenu(const MenuConfig &config) -> UiMenuPtr;

auto generateSpacer(const std::optional<olc::vi2d> &dims = {}) -> UiMenuPtr;
auto generateColoredSpacer(const olc::Pixel &color, const std::optional<olc::vi2d> &dims = {})
  -> UiMenuPtr;

} // namespace pge
