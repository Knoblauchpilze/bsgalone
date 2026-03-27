
#include "ScreenCommon.hh"

namespace bsgalone::client {

auto generateBlankHorizontalMenu(const std::optional<pge::Vec2i> &dims) -> ui::UiMenuPtr
{
  return generateBlankHorizontalMenu({}, dims);
}

auto generateBlankHorizontalMenu(const pge::Vec2i &pos, const std::optional<pge::Vec2i> &dims)
  -> ui::UiMenuPtr
{
  return generateBlankMenu(pos, ui::MenuLayout::HORIZONTAL, dims);
}

auto generateBlankVerticalMenu(const std::optional<pge::Vec2i> &dims) -> ui::UiMenuPtr
{
  return generateBlankVerticalMenu({}, dims);
}

auto generateBlankVerticalMenu(const pge::Vec2i &pos, const std::optional<pge::Vec2i> &dims)
  -> ui::UiMenuPtr
{
  return generateBlankMenu(pos, ui::MenuLayout::VERTICAL, dims);
}

auto generateBlankMenu(const pge::Vec2i &pos,
                       const ui::MenuLayout &layout,
                       const std::optional<pge::Vec2i> &dims) -> ui::UiMenuPtr
{
  ui::MenuConfig config{.pos = pos, .layout = layout};
  if (dims)
  {
    config.dims = *dims;
  }
  return generateBlankMenu(config);
}

auto generateBlankMenu(const ui::MenuConfig &config) -> ui::UiMenuPtr
{
  const auto bg = ui::bgConfigFromColor(pge::colors::BLANK);
  return std::make_unique<ui::UiMenu>(config, bg);
}

auto generateSpacer(const std::optional<pge::Vec2i> &dims) -> ui::UiMenuPtr
{
  return generateColoredSpacer(pge::colors::BLANK, dims);
}

auto generateColoredSpacer(const pge::Color &color, const std::optional<pge::Vec2i> &dims)
  -> ui::UiMenuPtr
{
  ui::MenuConfig config{.highlightable = false, .propagateEventsToChildren = false};
  if (dims)
  {
    config.dims = *dims;
  }
  const auto bg = ui::bgConfigFromColor(color);
  return std::make_unique<ui::UiMenu>(config, bg);
}

} // namespace bsgalone::client
