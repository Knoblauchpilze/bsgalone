
#pragma once

#include "olcEngine.hh"

namespace pge::menu {

/// @brief - Define the alignment of elements within a menu.
enum class Alignment
{
  Left,
  Center,
  Right
};

/// @brief - Define the order of elements within a menu.
enum class Ordering
{
  ImageFirst,
  TextFirst
};

/// @brief - Convenience structure defining the background that can be applied to
/// a menu. We either define a color that is applied on all the menu or a sprite
/// that is repeated a certain number of times.
struct MenuContentDesc
{
  std::string text;
  olc::Pixel color;
  olc::Pixel hColor;

  std::string icon;
  olc::vi2d size;

  Alignment align;
  Ordering order;

  bool expand;
};

/// @brief - Initializes a menu content structure defining only a text for the menu.
/// @param text - the text for this menu.
/// @param color - the color of the text.
/// @param align - the alignment for the text.
/// @return - the created menu object.
MenuContentDesc newTextContent(const std::string &text,
                               const olc::Pixel &color = olc::MAGENTA,
                               const Alignment &align  = Alignment::Left) noexcept;

/// @brief - Create a new menu content with a single size. We also need to provide
/// the size of the sprite.
/// @param sprite - the name of the file representing the sprite.
/// @param wrap - the dimensions of the sprite on screen.
/// @return - the created menu object.
MenuContentDesc newImageContent(const std::string &sprite, const olc::vi2d &size) noexcept;

/// @brief - Create a new background structure defining an image that is repeated
/// only once.
/// @param text - the text for this menu.
/// @param sprite - the name of the file representing the sprite.
/// @param size - the size of the sprite.
/// @param align - the alignment for the text.
/// @return - the created menu object.
MenuContentDesc newMenuContent(const std::string &text,
                               const std::string &sprite,
                               const olc::vi2d &size,
                               const Alignment &align = Alignment::Left) noexcept;

} // namespace pge::menu
