
#include "ScreenCommon.hh"

/// @brief - Ratio of the size of the menus compared to the total size of the window.
constexpr auto RATIO_MENU_TO_WINDOW = 0.6;

namespace pge {

auto generateDefaultScreen(const olc::vi2d &dims, const olc::Pixel &color) -> MenuShPtr
{
  olc::vi2d size(dims.x * RATIO_MENU_TO_WINDOW, dims.y * RATIO_MENU_TO_WINDOW);
  olc::vi2d pos(dims.x / 2.0f - size.x / 2.0f, dims.y / 2.0f - size.y / 2.0f);

  pge::menu::BackgroundDesc bg  = pge::menu::newColoredBackground(color);
  pge::menu::MenuContentDesc fg = pge::menu::newTextContent("");

  return std::make_shared<pge::Menu>(pos,
                                     size,
                                     "goMenu",
                                     bg,
                                     fg,
                                     pge::menu::Layout::Vertical,
                                     false,
                                     false);
}

auto generateScreenOption(const olc::vi2d &dims,
                          const std::string &text,
                          const olc::Pixel &bgColor,
                          const std::string &name,
                          bool selectable) -> MenuShPtr
{
  auto bg   = pge::menu::newColoredBackground(bgColor);
  bg.hColor = olc::GREY;

  auto fd   = pge::menu::newMenuContent(text, "", dims);
  fd.color  = olc::WHITE;
  fd.hColor = olc::BLACK;
  fd.align  = pge::menu::Alignment::Center;

  return std::make_shared<pge::Menu>(olc::vi2d(),
                                     dims,
                                     name,
                                     bg,
                                     fd,
                                     pge::menu::Layout::Horizontal,
                                     selectable,
                                     false);
}

namespace {
auto generateForegroundDesc(const std::string &text,
                            const olc::vi2d &size,
                            const olc::Pixel &bgColor,
                            const std::optional<olc::Pixel> &textColor) -> menu::MenuContentDesc
{
  auto fd = menu::newMenuContent(text, "", size);

  if (textColor)
  {
    fd.color  = *textColor;
    fd.hColor = modulate(fd.color, 2.0f);
  }
  else
  {
    fd.color  = bgColor == olc::BLACK ? olc::WHITE : olc::BLACK;
    fd.hColor = RGBToHSL(bgColor).b < 128 ? olc::WHITE : olc::BLACK;
  }

  fd.align = menu::Alignment::Center;

  return fd;
}
} // namespace

auto generateMenu(const olc::vi2d &pos,
                  const olc::vi2d &size,
                  const std::string &text,
                  const std::string &name,
                  olc::Pixel bgColor,
                  const std::optional<olc::Pixel> &textColor,
                  bool clickable,
                  bool selectable,
                  const menu::Layout &layout) -> pge::MenuShPtr
{
  const auto fd = generateForegroundDesc(text, size, bgColor, textColor);
  return std::make_shared<pge::Menu>(pos,
                                     size,
                                     name,
                                     menu::newColoredBackground(bgColor),
                                     fd,
                                     layout,
                                     clickable,
                                     selectable);
}

auto generateSlotMenu(const olc::vi2d &pos,
                      const olc::vi2d &size,
                      const std::string &text,
                      const std::string &name,
                      olc::Pixel bgColor,
                      const std::optional<olc::Pixel> &textColor,
                      bool clickable,
                      bool selectable,
                      const menu::Layout &layout) -> pge::MenuShPtr
{
  const auto fd = generateForegroundDesc(text, size, bgColor, textColor);
  return std::make_shared<pge::Menu>(pos,
                                     size,
                                     name,
                                     menu::newColoredBackground(bgColor),
                                     fd,
                                     layout,
                                     clickable,
                                     selectable,
                                     true);
}

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
  MenuConfig config{.pos = pos, .layout = layout};
  if (dims)
  {
    config.dims = *dims;
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
  MenuConfig config{.highlightable = false, .propagateEventsToChildren = false};
  if (dims)
  {
    config.dims = *dims;
  }
  const auto bg = bgConfigFromColor(olc::BLANK);
  return std::make_unique<UiMenu>(config, bg);
}

} // namespace pge
