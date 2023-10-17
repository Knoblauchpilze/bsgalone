
#pragma once

#include "Menu.hh"

namespace pge {

inline Menu::~Menu()
{
  clearContent();
  clear();
}

inline bool Menu::visible() const noexcept
{
  return m_state.visible;
}

inline void Menu::setVisible(bool visible) noexcept
{
  m_state.visible = visible;
}

inline void Menu::setClickable(bool click) noexcept
{
  m_state.clickable = click;
}

inline void Menu::setSelectable(bool select) noexcept
{
  m_state.selectable = select;
}

inline void Menu::setEnabled(bool enabled) noexcept
{
  m_state.enabled = enabled;
}

inline olc::Pixel Menu::getBackgroundColor() const noexcept
{
  return m_bg.color;
}

inline olc::vf2d Menu::getSize() const noexcept
{
  return m_size;
}

inline std::string Menu::getText() const noexcept
{
  return m_fg.text;
}

inline void Menu::setBackground(const menu::BackgroundDesc &bg)
{
  m_bg = bg;

  // Update the parent's display if possible.
  if (m_parent != nullptr)
  {
    m_parent->updateChildren();
  }
}

inline void Menu::setBackgroundColor(const olc::Pixel &color)
{
  menu::updateFromColor(m_bg, color);

  if (m_parent != nullptr)
  {
    m_parent->updateChildren();
  }
}

inline void Menu::setContent(const menu::MenuContentDesc &mcd)
{
  clearContent();
  m_fg = mcd;
  loadFGTile();

  // Update the parent's display if possible.
  if (m_parent != nullptr)
  {
    m_parent->updateChildren();
  }
}

inline void Menu::setText(const std::string &text)
{
  m_fg.text = text;

  // Update the parent's display if possible.
  if (m_parent != nullptr)
  {
    m_parent->updateChildren();
  }
}

inline void Menu::onClick(std::vector<ActionShPtr> &actions) const
{
  // Trigger the callback if it is defined.
  if (m_callback)
  {
    m_callback(actions);
  }
}

inline bool Menu::onHighlight() const
{
  return m_state.enabled;
}

inline olc::vi2d Menu::absolutePosition() const noexcept
{
  olc::vi2d p(0, 0);

  if (m_parent != nullptr)
  {
    p = m_parent->absolutePosition();
  }

  p += m_pos;

  return p;
}

inline menu::BackgroundDesc Menu::bg() const noexcept
{
  return m_bg;
}

inline menu::MenuContentDesc Menu::fg() const noexcept
{
  return m_fg;
}

inline void Menu::clear() {}

inline void Menu::clearContent()
{
  if (m_fgSprite != nullptr)
  {
    delete m_fgSprite;
  }

  m_fgSprite = nullptr;
}

} // namespace pge

#pragma once
