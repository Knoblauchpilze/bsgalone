
#pragma once

#include "Menu.hh"
#include <optional>
#include <string>

namespace pge {

auto generateDefaultScreen(const olc::vi2d &dims, const olc::Pixel &color) -> MenuShPtr;

auto generateScreenOption(const olc::vi2d &dims,
                          const std::string &text,
                          const olc::Pixel &bgColor,
                          const std::string &name,
                          bool selectable) -> MenuShPtr;

auto generateMenu(const olc::vi2d &pos,
                  const olc::vi2d &size,
                  const std::string &text,
                  const std::string &name,
                  olc::Pixel bgColor                         = olc::VERY_DARK_GREEN,
                  const std::optional<olc::Pixel> &textColor = {},
                  bool clickable                             = false,
                  bool selectable                            = false,
                  const menu::Layout &layout = menu::Layout::Vertical) -> pge::MenuShPtr;

auto generateSlotMenu(const olc::vi2d &pos,
                      const olc::vi2d &size,
                      const std::string &text,
                      const std::string &name,
                      olc::Pixel bgColor                         = olc::VERY_DARK_GREEN,
                      const std::optional<olc::Pixel> &textColor = {},
                      bool clickable                             = false,
                      bool selectable                            = false,
                      const menu::Layout &layout = menu::Layout::Vertical) -> pge::MenuShPtr;

} // namespace pge
