
#include "ScreenCommon.hh"
#include "VectorConversion.hh"

namespace pge {

auto generateBlankHorizontalMenu(const std::optional<olc::vi2d> &dims) -> UiMenuPtr
{
  return generateBlankHorizontalMenu(olc::vi2d{}, dims);
}

auto generateBlankHorizontalMenu(const olc::vi2d &pos, const std::optional<olc::vi2d> &dims)
  -> UiMenuPtr
{
  return generateBlankMenu(pos, MenuLayout::HORIZONTAL, dims);
}

auto generateBlankVerticalMenu(const std::optional<olc::vi2d> &dims) -> UiMenuPtr
{
  return generateBlankVerticalMenu(olc::vi2d{}, dims);
}

auto generateBlankVerticalMenu(const olc::vi2d &pos, const std::optional<olc::vi2d> &dims)
  -> UiMenuPtr
{
  return generateBlankMenu(pos, MenuLayout::VERTICAL, dims);
}

auto generateBlankMenu(const olc::vi2d &pos,
                       const MenuLayout &layout,
                       const std::optional<olc::vi2d> &dims) -> UiMenuPtr
{
  MenuConfig config{.pos = toVec2i(pos), .layout = layout};
  if (dims)
  {
    config.dims = toVec2i(*dims);
  }
  return generateBlankMenu(config);
}

auto generateBlankMenu(const MenuConfig &config) -> UiMenuPtr
{
  const auto bg = bgConfigFromColor(olc::BLANK);
  return std::make_unique<UiMenu>(config, bg);
}

auto generateSpacer(const std::optional<olc::vi2d> &dims) -> UiMenuPtr
{
  return generateColoredSpacer(olc::BLANK, dims);
}

auto generateColoredSpacer(const olc::Pixel &color, const std::optional<olc::vi2d> &dims)
  -> UiMenuPtr
{
  MenuConfig config{.highlightable = false, .propagateEventsToChildren = false};
  if (dims)
  {
    config.dims = toVec2i(*dims);
  }
  const auto bg = bgConfigFromColor(color);
  return std::make_unique<UiMenu>(config, bg);
}

} // namespace pge
