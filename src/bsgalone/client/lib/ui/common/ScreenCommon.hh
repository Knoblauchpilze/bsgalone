
#pragma once

#include "UiMenu.hh"
#include <optional>
#include <string>

namespace pge {

auto generateBlankHorizontalMenu(const std::optional<Vec2i> &dims = {}) -> ui::UiMenuPtr;
auto generateBlankHorizontalMenu(const Vec2i &pos, const std::optional<Vec2i> &dims = {})
  -> ui::UiMenuPtr;
auto generateBlankVerticalMenu(const std::optional<Vec2i> &dims = {}) -> ui::UiMenuPtr;
auto generateBlankVerticalMenu(const Vec2i &pos, const std::optional<Vec2i> &dims = {})
  -> ui::UiMenuPtr;
auto generateBlankMenu(const Vec2i &pos,
                       const ui::MenuLayout &layout,
                       const std::optional<Vec2i> &dims = {}) -> ui::UiMenuPtr;
auto generateBlankMenu(const ui::MenuConfig &config) -> ui::UiMenuPtr;

auto generateSpacer(const std::optional<Vec2i> &dims = {}) -> ui::UiMenuPtr;
auto generateColoredSpacer(const Color &color, const std::optional<Vec2i> &dims = {})
  -> ui::UiMenuPtr;

} // namespace pge
