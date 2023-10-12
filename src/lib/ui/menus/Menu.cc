
#include "Menu.hh"

namespace pge {

Menu::Menu(const olc::vi2d &pos,
           const olc::vi2d &size,
           const std::string &name,
           const menu::BackgroundDesc &bg,
           const menu::MenuContentDesc &fg,
           const menu::Layout &layout,
           bool clickable,
           bool selectable,
           Menu *parent)
  : utils::CoreObject(name)
  ,

  m_state(State{
    true,       // visible
    true,       // enabled
    clickable,  // clickable
    selectable, // selectable
    false,      // highlighted
    false       // selected
  })
  ,

  m_pos(pos)
  , m_size(size)
  ,

  m_bg(bg)
  , m_fg(fg)
  , m_fgSprite(nullptr)
  ,

  m_layout(layout)
  ,

  m_parent(parent)
  , m_children()
  ,

  m_callback()
{
  setService("menu");

  loadFGTile();
}

void Menu::render(olc::PixelGameEngine *pge) const
{
  // If the menu is not visible, do nothing.
  if (!m_state.visible)
  {
    return;
  }

  // Render the uniform background for this menu.
  olc::vi2d pos = absolutePosition();
  olc::Pixel c  = m_bg.color;
  if ((m_state.clickable && m_state.highlighted) || (m_state.selectable && m_state.selected))
  {
    c = m_bg.hColor;
  }
  pge->FillRectDecal(pos, m_size, c);

  // Render this menu.
  renderSelf(pge);

  // And then draw children in the order there were
  // added: it means that the last added menu will
  // be repainted on top of the others.
  for (unsigned id = 0u; id < m_children.size(); ++id)
  {
    m_children[id]->render(pge);
  }
}

menu::InputHandle Menu::processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions)
{
  menu::InputHandle res{false, false};

  // In case the menu is not visible, do nothing.
  if (!m_state.visible)
  {
    return res;
  }

  // Make sure that the children get their chance
  // to process the event.
  for (unsigned id = 0u; id < m_children.size(); ++id)
  {
    menu::InputHandle rc = m_children[id]->processUserInput(c, actions);

    res.relevant = res.relevant || rc.relevant;
    res.selected = res.selected || rc.selected;
  }

  // If the mouse is not inside this element, stop
  // the process here: children still got a chance
  // to update their state with this event. And no
  // matter the `used` value, we know that we're
  // not highlighted anymore at this step if the
  // following conditions apply: it either mean
  // that the mouse is not inside this menu or
  // that a child is more relevant than we are.
  bool click = (c.buttons[controls::mouse::LEFT] == controls::ButtonState::RELEASED);

  olc::vi2d ap = absolutePosition();
  if (c.mPosX < ap.x || c.mPosX >= ap.x + m_size.x || c.mPosY < ap.y || c.mPosY >= ap.y + m_size.y
      || res.relevant || res.selected)
  {
    m_state.highlighted = false;

    if (res.selected || click)
    {
      m_state.selected = false;
    }

    return res;
  }

  // This menu is now highlighted. We also set
  // the return value to indicate that this
  // event was indeed relevant.
  bool process        = onHighlight();
  m_state.highlighted = process;
  res.relevant        = true;

  // In case the user clicks on the menu, we need
  // to trigger the corresponding handler.
  if (click)
  {
    // Only trigger the `onClick` in case the user
    // indicated to do so when building the menu.
    if (process)
    {
      onClick(actions);
    }

    // But always register the internal state as
    // selected.
    m_state.selected = true;
    res.selected     = true;
  }

  return res;
}

void Menu::addMenu(MenuShPtr child)
{
  // Check consistency.
  if (child == nullptr)
  {
    return;
  }

  // Assign this element has the parent of the new
  // child and register it as a new submenu.
  // Note that this might cause issue in case the
  // child already has a parent.
  if (child->m_parent != nullptr)
  {
    log("Reparenting menu \"" + child->getName() + "\" from \"" + child->m_parent->getName() + "\"",
        utils::Level::Warning);
  }

  child->m_parent = this;

  m_children.push_back(child);

  // Update properties of each child in response
  // to the new child.
  updateChildren();
}

void Menu::setAction(menu::RegisterAction action)
{
  m_callback = action;
}

void Menu::setSimpleAction(action::Process process)
{
  m_callback = [process](std::vector<ActionShPtr> &actions) {
    actions.push_back(std::make_shared<Action>(process));
  };
}

void Menu::renderSelf(olc::PixelGameEngine *pge) const
{
  // We need to display both the text and the icon
  // if needed. We assume the content will always
  // be centered along the perpendicular axis for
  // this menu.
  // Depending on whether we need to display both
  // an image or a text and in which order layout
  // in the menu will change.
  // Also, in case there's no text nor sprite to
  // display we can return right now.
  if (m_fg.text == "" && m_fgSprite == nullptr)
  {
    return;
  }

  olc::vi2d ap = absolutePosition();

  if (m_fg.text != "" && m_fgSprite == nullptr)
  {
    olc::vi2d ts = pge->GetTextSize(m_fg.text);

    olc::vi2d p;

    switch (m_fg.align)
    {
      case menu::Alignment::Center:
        p = olc::vi2d(static_cast<int>(ap.x + (m_size.x - ts.x) / 2.0f),
                      static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f));
        break;
      case menu::Alignment::Right:
        p = olc::vi2d(ap.x + m_size.x - ts.x, static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f));
        break;
      case menu::Alignment::Left:
      default:
        p = olc::vi2d(ap.x, static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f));
        break;
    }

    olc::Pixel c = m_fg.color;
    if ((m_state.clickable && m_state.highlighted) || (m_state.selectable && m_state.selected))
    {
      c = m_fg.hColor;
    }

    pge->DrawStringDecal(p, m_fg.text, c);

    return;
  }

  if (m_fg.text == "" && m_fgSprite != nullptr)
  {
    // Center the image if it is the only element
    // to display.
    olc::vi2d p(static_cast<int>(ap.x + m_size.x / 2.0f - m_fg.size.x / 2.0f),
                static_cast<int>(ap.y + m_size.y / 2.0f - m_fg.size.y / 2.0f));

    olc::vi2d ss(m_fgSprite->sprite->width, m_fgSprite->sprite->height);
    olc::vf2d s(1.0f * m_fg.size.x / ss.x, 1.0f * m_fg.size.y / ss.y);

    pge->DrawPartialDecal(p, m_fgSprite, olc::vi2d(), ss, s);

    return;
  }

  // Both text and icon should be displayed: the
  // order is specified in the content description.
  olc::vi2d ts = pge->GetTextSize(m_fg.text);
  olc::vi2d cs = ts + m_fg.size;

  olc::vi2d tp;
  olc::vi2d sp;

  olc::vi2d ss(m_fgSprite->sprite->width, m_fgSprite->sprite->height);
  olc::vf2d s(1.0f * m_fg.size.x / ss.x, 1.0f * m_fg.size.y / ss.y);

  switch (m_fg.order)
  {
    case menu::Ordering::TextFirst:
      switch (m_fg.align)
      {
        case menu::Alignment::Center:
          tp = olc::vi2d(static_cast<int>(ap.x + (m_size.x - cs.x) / 2.0f),
                         static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f));

          sp = olc::vi2d(static_cast<int>(ap.x + (m_size.x - cs.x) / 2.0f + ts.x),
                         static_cast<int>(ap.y + m_size.y / 2.0f - m_fg.size.y / 2.0f));
          break;
        case menu::Alignment::Right:
          tp = olc::vi2d(ap.x + m_size.x - cs.x, static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f));

          sp = olc::vi2d(ap.x + m_size.x - m_fg.size.x,
                         static_cast<int>(ap.y + m_size.y / 2.0f - m_fg.size.y / 2.0f));
          break;
        case menu::Alignment::Left:
        default:
          tp = olc::vi2d(ap.x, static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f));

          sp = olc::vi2d(ap.x + ts.x, static_cast<int>(ap.y + m_size.y / 2.0f - m_fg.size.y / 2.0f));
          break;
      }
      break;
    case menu::Ordering::ImageFirst:
    default:
      switch (m_fg.align)
      {
        case menu::Alignment::Center:
          tp = olc::vi2d(static_cast<int>(ap.x + (m_size.x - cs.x) / 2.0f + m_fg.size.x),
                         static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f));

          sp = olc::vi2d(static_cast<int>(ap.x + (m_size.x - cs.x) / 2.0f),
                         static_cast<int>(ap.y + m_size.y / 2.0f - m_fg.size.y / 2.0f));
          break;
        case menu::Alignment::Right:
          tp = olc::vi2d(ap.x + m_size.x - ts.x, static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f));

          sp = olc::vi2d(ap.x + m_size.x - cs.x,
                         static_cast<int>(ap.y + m_size.y / 2.0f - m_fg.size.y / 2.0f));
          break;
        case menu::Alignment::Left:
        default:
          tp = olc::vi2d(ap.x + m_fg.size.x, static_cast<int>(ap.y + (m_size.y - ts.y) / 2.0f));

          sp = olc::vi2d(ap.x, static_cast<int>(ap.y + m_size.y / 2.0f - m_fg.size.y / 2.0f));
          break;
      }
      break;
  }

  // Draw both the text and the image.
  olc::Pixel c = m_fg.color;
  if ((m_state.clickable && m_state.highlighted) || (m_state.selectable && m_state.selected))
  {
    c = m_fg.hColor;
  }

  pge->DrawStringDecal(tp, m_fg.text, c);

  pge->DrawPartialDecal(sp, m_fgSprite, olc::vi2d(), ss, s);
}

void Menu::loadFGTile()
{
  // Check for actually needing to load anything.
  if (m_fg.icon == "")
  {
    return;
  }

  // Clamp the size of the icon.
  m_fg.size.x = std::max(m_fg.size.x, 10);
  m_fg.size.y = std::max(m_fg.size.y, 10);

  // Load the sprite.
  olc::Sprite *spr = new olc::Sprite(m_fg.icon);
  m_fgSprite       = new olc::Decal(spr);
}

void Menu::updateChildren()
{
  // Update the size based on the layout for this
  // menu: we also have to update the other items
  // so that a consistent size is defined.
  int wh = (m_layout == menu::Layout::Horizontal ? m_size.y : m_size.x);
  int i  = (m_layout == menu::Layout::Horizontal ? m_size.x : m_size.y);

  // Compute the size to allocate to each child
  // based on the expanding ones.
  int expandableSize = i;
  int count          = m_children.size();
  for (unsigned id = 0u; id < m_children.size(); ++id)
  {
    if (!m_children[id]->m_fg.expand)
    {
      expandableSize -= (m_layout == menu::Layout::Horizontal ? m_children[id]->m_size.x
                                                              : m_children[id]->m_size.y);

      --count;
    }
  }

  if (expandableSize < 0)
  {
    log("Menu has " + std::to_string(m_children.size()) + " child(ren)" + " occpupying "
          + std::to_string(i - expandableSize) + " but menu is only " + std::to_string(i)
          + ", truncation will occur",
        utils::Level::Warning);

    expandableSize = 0;
  }
  count = std::max(count, 1);

  float d = 1.0f * expandableSize / count;

  // Note that we also force the wraping for the
  // background tiled children to something that
  // will be consistent with their actual size.
  float offset = 0.0f;
  float delta  = 0.0f;

  for (unsigned id = 0u; id < m_children.size(); ++id)
  {
    switch (m_layout)
    {
      case menu::Layout::Vertical:
        m_children[id]->m_size.x = std::min(m_children[id]->m_size.x, wh);
        if (m_children[id]->m_bg.scale)
        {
          m_children[id]->m_size.x = wh;
        }
        if (m_children[id]->m_fg.expand)
        {
          m_children[id]->m_size.y = static_cast<int>(d);

          // Make sure that the size of the child is
          // a bit larger in case the `d` is not a
          // perfect integer.
          if (id == m_children.size() - 1)
          {
            delta += (d - m_children[id]->m_size.y);
          }
          if (delta > 1.0f)
          {
            int sup = static_cast<int>(std::floor(delta));
            m_children[id]->m_size.y += sup;
          }

          if (id != m_children.size())
          {
            delta += (d - m_children[id]->m_size.y);
          }
        }

        m_children[id]->m_pos.x = (wh - m_children[id]->m_size.x) / 2.0f;
        m_children[id]->m_pos.y = offset;

        offset += m_children[id]->m_size.y;

        // We want the icon to occupy all the space
        // available for the child.
        if (m_children[id]->m_fg.icon != "")
        {
          m_children[id]->m_fg.size.y = m_children[id]->m_size.y;
        }
        break;
      case menu::Layout::Horizontal:
      default:
        if (m_children[id]->m_fg.expand)
        {
          m_children[id]->m_size.x = static_cast<int>(d);

          // Make sure that the size of the child is
          // a bit larger in case the `d` is not a
          // perfect integer.
          if (id == m_children.size() - 1)
          {
            delta += (d - m_children[id]->m_size.x);
          }
          if (delta > 1.0f)
          {
            int sup = static_cast<int>(std::floor(delta));
            m_children[id]->m_size.x += sup;
          }

          if (id != m_children.size())
          {
            delta += (d - m_children[id]->m_size.x);
          }
        }
        m_children[id]->m_size.y = std::min(m_children[id]->m_size.y, wh);
        if (m_children[id]->m_bg.scale)
        {
          m_children[id]->m_size.y = wh;
        }

        m_children[id]->m_pos.x = offset;
        m_children[id]->m_pos.y = (wh - m_children[id]->m_size.y) / 2.0f;

        offset += m_children[id]->m_size.x;

        // Note that we cannot force the horizontal
        // size of the icon in this case because we
        // display the text and icon always in the
        // horizontal direction: so if we wanted to
        // set some sort of maximum size it should
        // be consistent with allowing the text to
        // be fully visible or something.
        // If we wanted to have an internal layout
        // (which would indicate whether the icon
        // and text are laid out horizontally or
        // vertically) we could adapt the size of
        // the icon here.
        break;
    }
  }
}

} // namespace pge
