
#pragma once

#include "UiMenu.hh"
#include <optional>
#include <string>

namespace bsgalone::client {

auto generateBlankHorizontalMenu(const std::optional<pge::Vec2i> &dims = {}) -> ui::UiMenuPtr;
auto generateBlankHorizontalMenu(const pge::Vec2i &pos, const std::optional<pge::Vec2i> &dims = {})
  -> ui::UiMenuPtr;
auto generateBlankVerticalMenu(const std::optional<pge::Vec2i> &dims = {}) -> ui::UiMenuPtr;
auto generateBlankVerticalMenu(const pge::Vec2i &pos, const std::optional<pge::Vec2i> &dims = {})
  -> ui::UiMenuPtr;
auto generateBlankMenu(const pge::Vec2i &pos,
                       const ui::MenuLayout &layout,
                       const std::optional<pge::Vec2i> &dims = {}) -> ui::UiMenuPtr;
auto generateBlankMenu(const ui::MenuConfig &config) -> ui::UiMenuPtr;

auto generateSpacer(const std::optional<pge::Vec2i> &dims = {}) -> ui::UiMenuPtr;
auto generateColoredSpacer(const pge::Color &color, const std::optional<pge::Vec2i> &dims = {})
  -> ui::UiMenuPtr;

} // namespace bsgalone::client
