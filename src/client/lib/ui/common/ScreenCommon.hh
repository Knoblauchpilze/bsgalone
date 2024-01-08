
#pragma once

#include "UiMenu.hh"
#include <optional>
#include <string>

namespace pge {

auto generateBlankHorizontalMenu(const std::optional<Vec2i> &dims = {}) -> UiMenuPtr;
auto generateBlankHorizontalMenu(const Vec2i &pos, const std::optional<Vec2i> &dims = {})
  -> UiMenuPtr;
auto generateBlankVerticalMenu(const std::optional<Vec2i> &dims = {}) -> UiMenuPtr;
auto generateBlankVerticalMenu(const Vec2i &pos, const std::optional<Vec2i> &dims = {})
  -> UiMenuPtr;
auto generateBlankMenu(const Vec2i &pos,
                       const MenuLayout &layout,
                       const std::optional<Vec2i> &dims = {}) -> UiMenuPtr;
auto generateBlankMenu(const MenuConfig &config) -> UiMenuPtr;

auto generateSpacer(const std::optional<Vec2i> &dims = {}) -> UiMenuPtr;
auto generateColoredSpacer(const olc::Pixel &color, const std::optional<Vec2i> &dims = {})
  -> UiMenuPtr;

} // namespace pge
