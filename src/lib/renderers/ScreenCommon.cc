
#include "ScreenCommon.hh"

/// @brief - Ratio of the size of the menus compared to the total size of the window.
#define RATIO_MENU_TO_WINDOW 0.6

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
  pge::menu::BackgroundDesc bg = pge::menu::newColoredBackground(bgColor);
  bg.hColor                    = olc::GREY;

  pge::menu::MenuContentDesc fd = pge::menu::newMenuContent(text, "", dims);
  fd.color                      = olc::WHITE;
  fd.hColor                     = olc::BLACK;
  fd.align                      = pge::menu::Alignment::Center;

  return std::make_shared<pge::Menu>(olc::vi2d(),
                                     dims,
                                     name,
                                     bg,
                                     fd,
                                     pge::menu::Layout::Horizontal,
                                     selectable,
                                     false);
}

} // namespace pge
