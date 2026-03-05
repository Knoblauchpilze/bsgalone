
#include "ScreenCommon.hh"

namespace pge {

auto generateBlankHorizontalMenu(const std::optional<Vec2i> &dims) -> ui::UiMenuPtr
{
  return generateBlankHorizontalMenu({}, dims);
}

auto generateBlankHorizontalMenu(const Vec2i &pos, const std::optional<Vec2i> &dims)
  -> ui::UiMenuPtr
{
  return generateBlankMenu(pos, ui::MenuLayout::HORIZONTAL, dims);
}

auto generateBlankVerticalMenu(const std::optional<Vec2i> &dims) -> ui::UiMenuPtr
{
  return generateBlankVerticalMenu({}, dims);
}

auto generateBlankVerticalMenu(const Vec2i &pos, const std::optional<Vec2i> &dims) -> ui::UiMenuPtr
{
  return generateBlankMenu(pos, ui::MenuLayout::VERTICAL, dims);
}

auto generateBlankMenu(const Vec2i &pos,
                       const ui::MenuLayout &layout,
                       const std::optional<Vec2i> &dims) -> ui::UiMenuPtr
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
  const auto bg = ui::bgConfigFromColor(colors::BLANK);
  return std::make_unique<ui::UiMenu>(config, bg);
}

auto generateSpacer(const std::optional<Vec2i> &dims) -> ui::UiMenuPtr
{
  return generateColoredSpacer(colors::BLANK, dims);
}

auto generateColoredSpacer(const Color &color, const std::optional<Vec2i> &dims) -> ui::UiMenuPtr
{
  ui::MenuConfig config{.highlightable = false, .propagateEventsToChildren = false};
  if (dims)
  {
    config.dims = *dims;
  }
  const auto bg = ui::bgConfigFromColor(color);
  return std::make_unique<ui::UiMenu>(config, bg);
}

} // namespace pge
