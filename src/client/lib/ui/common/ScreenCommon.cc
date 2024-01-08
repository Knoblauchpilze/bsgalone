
#include "ScreenCommon.hh"

namespace pge {

auto generateBlankHorizontalMenu(const std::optional<Vec2i> &dims) -> UiMenuPtr
{
  return generateBlankHorizontalMenu({}, dims);
}

auto generateBlankHorizontalMenu(const Vec2i &pos, const std::optional<Vec2i> &dims) -> UiMenuPtr
{
  return generateBlankMenu(pos, MenuLayout::HORIZONTAL, dims);
}

auto generateBlankVerticalMenu(const std::optional<Vec2i> &dims) -> UiMenuPtr
{
  return generateBlankVerticalMenu({}, dims);
}

auto generateBlankVerticalMenu(const Vec2i &pos, const std::optional<Vec2i> &dims) -> UiMenuPtr
{
  return generateBlankMenu(pos, MenuLayout::VERTICAL, dims);
}

auto generateBlankMenu(const Vec2i &pos, const MenuLayout &layout, const std::optional<Vec2i> &dims)
  -> UiMenuPtr
{
  MenuConfig config{.pos = pos, .layout = layout};
  if (dims)
  {
    config.dims = *dims;
  }
  return generateBlankMenu(config);
}

auto generateBlankMenu(const MenuConfig &config) -> UiMenuPtr
{
  const auto bg = bgConfigFromColor(colors::BLANK);
  return std::make_unique<UiMenu>(config, bg);
}

auto generateSpacer(const std::optional<Vec2i> &dims) -> UiMenuPtr
{
  return generateColoredSpacer(colors::BLANK, dims);
}

auto generateColoredSpacer(const Color &color, const std::optional<Vec2i> &dims) -> UiMenuPtr
{
  MenuConfig config{.highlightable = false, .propagateEventsToChildren = false};
  if (dims)
  {
    config.dims = *dims;
  }
  const auto bg = bgConfigFromColor(color);
  return std::make_unique<UiMenu>(config, bg);
}

} // namespace pge
