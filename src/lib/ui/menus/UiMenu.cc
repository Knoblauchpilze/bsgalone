
#include "UiMenu.hh"

namespace pge {

UiMenu::UiMenu(const MenuConfig &config, const BackgroundConfig &bg)
  : utils::CoreObject("menu")
  , m_bg(bg)
{
  initializeFromConfig(config);
}

bool UiMenu::visible() const noexcept
{
  return m_state.visible;
}

void UiMenu::setVisible(const bool visible) noexcept
{
  m_state.visible = visible;
}

void UiMenu::addMenu(UiMenuPtr child)
{
  if (child->m_parent != nullptr)
  {
    warn("Reparenting menu \"" + child->getName() + "\" from \"" + child->m_parent->getName()
         + "\"");
  }

  child->m_parent = this;
  m_children.push_back(std::move(child));
  updateLayoutAfterChildChange();
}

void UiMenu::updateBgColor(const olc::Pixel &color)
{
  m_bg = bgConfigFromColor(color);
}

void UiMenu::render(olc::PixelGameEngine *pge) const
{
  if (!m_state.visible)
  {
    return;
  }

  renderSelf(pge);
  for (const auto &child : m_children)
  {
    child->render(pge);
  }
}

bool UiMenu::processUserInput(UserInputData &inputData)
{
  if (!m_state.visible)
  {
    return false;
  }

  auto inputRelevantForChildren{false};
  if (m_state.propagateEventsToChildren)
  {
    for (const auto &child : m_children)
    {
      inputRelevantForChildren |= child->processUserInput(inputData);
    }
  }

  const olc::vi2d mPos{inputData.controls.mPosX, inputData.controls.mPosY};
  if (!isWithinMenu(mPos) || inputRelevantForChildren)
  {
    m_state.highlighted = false;

    if (!inputRelevantForChildren && inputData.controls.released(controls::mouse::LEFT)
        && m_lostFocusCallback)
    {
      (*m_lostFocusCallback)();
    }

    return inputRelevantForChildren;
  }

  onRelevantInput(inputData);

  return true;
}

auto UiMenu::absolutePosition() const noexcept -> olc::vi2d
{
  auto p = m_pos;
  if (m_parent != nullptr)
  {
    p += m_parent->absolutePosition();
  }
  return p;
}

auto UiMenu::dims() const noexcept -> olc::vi2d
{
  return m_dims;
}

auto UiMenu::state() const noexcept -> const State &
{
  return m_state;
}

void UiMenu::setClickCallback(const ClickCallback &callback)
{
  m_clickCallback = callback;
}

void UiMenu::setLostFocusCallback(const ClickCallback &callback)
{
  m_lostFocusCallback = callback;
}

void UiMenu::renderCustom(olc::PixelGameEngine * /*pge*/) const
{
  // Intentionally empty to allow subclassing.
}

void UiMenu::initializeFromConfig(const MenuConfig &config)
{
  m_pos  = config.pos;
  m_dims = config.dims;

  m_layout              = config.layout;
  m_expandChildrenToFit = config.expandChildrenToFit;

  m_state.visible                   = config.visible;
  m_state.higlightable              = config.highlightable;
  m_state.propagateEventsToChildren = config.propagateEventsToChildren;

  m_highlightCallback = config.highlightCallback;
  m_clickCallback     = config.clickCallback;
  m_gameClickCallback = config.gameClickCallback;
  m_lostFocusCallback = config.lostFocusCallback;
}

void UiMenu::renderSelf(olc::PixelGameEngine *pge) const
{
  const auto absPos = absolutePosition();
  const auto color  = getColorFromState();
  pge->FillRectDecal(absPos, m_dims, color);
  renderCustom(pge);
}

auto UiMenu::getColorFromState() const -> olc::Pixel
{
  if (m_state.higlightable && m_state.highlighted)
  {
    return m_bg.hColor;
  }
  return m_bg.color;
}

bool UiMenu::isWithinMenu(const olc::vi2d &pos) const
{
  const auto absPos = absolutePosition();
  const auto minX   = absPos.x;
  const auto minY   = absPos.y;
  const auto maxX   = absPos.x + m_dims.x;
  const auto maxY   = absPos.y + m_dims.y;

  if (pos.x < minX || pos.x > maxX)
  {
    return false;
  }
  if (pos.y < minY || pos.y > maxY)
  {
    return false;
  }

  return true;
}

void UiMenu::onRelevantInput(UserInputData &inputData)
{
  m_state.highlighted = true;

  if (m_highlightCallback)
  {
    (*m_highlightCallback)();
  }

  if (inputData.controls.released(controls::mouse::LEFT))
  {
    if (m_clickCallback)
    {
      (*m_clickCallback)();
    }
    if (m_gameClickCallback)
    {
      inputData.actions.emplace_back(std::make_unique<Action>(*m_gameClickCallback));
    }
  }
}

void UiMenu::updateLayoutAfterChildChange()
{
  const auto sizeToDistribute = (m_layout == MenuLayout::HORIZONTAL ? m_dims.x : m_dims.y);
  const auto count            = static_cast<int>(m_children.size());
  const auto sizeForEachChild = 1.0f * sizeToDistribute / count;

  switch (m_layout)
  {
    case MenuLayout::HORIZONTAL:
      adaptChildrenToMatchHorizontalSize(sizeForEachChild);
      break;
    case MenuLayout::VERTICAL:
      adaptChildrenToMatchVerticalSize(sizeForEachChild);
      break;
    default:
      error("Failed to update layout",
            "Unknown layout " + std::to_string(static_cast<int>(m_layout)));
      break;
  }

  for (const auto &child : m_children)
  {
    child->updateLayoutAfterChildChange();
  }
}

namespace {
auto floorToInt(const float v) -> int
{
  return static_cast<int>(std::floor(v));
}

auto roundToInt(const float v) -> int
{
  return static_cast<int>(std::round(v));
}
} // namespace

void UiMenu::adaptChildrenToMatchHorizontalSize(const float desiredXSize)
{
  auto delta  = 0.0f;
  auto offset = 0.0f;

  for (const auto &child : m_children)
  {
    child->m_dims.x = floorToInt(desiredXSize);
    delta += (desiredXSize - child->m_dims.x);
    if (m_expandChildrenToFit)
    {
      child->m_dims.y = m_dims.y;
    }
    else
    {
      child->m_dims.y = std::min(m_dims.y, child->m_dims.y);
    }

    if (delta > 1.0f)
    {
      const auto additionalSize = floorToInt(delta);
      child->m_dims.x += additionalSize;
      delta -= additionalSize;
    }

    child->m_pos.x = offset;
    child->m_pos.y = (m_dims.y - child->m_dims.y) / 2;

    offset += child->m_dims.x;
  }

  if (delta > 0.0f)
  {
    m_children.back()->m_dims.x += roundToInt(delta);
  }
}

void UiMenu::adaptChildrenToMatchVerticalSize(const float desiredYSize)
{
  auto delta  = 0.0f;
  auto offset = 0.0f;

  for (const auto &child : m_children)
  {
    if (m_expandChildrenToFit)
    {
      child->m_dims.x = m_dims.x;
    }
    else
    {
      child->m_dims.x = std::min(m_dims.x, child->m_dims.x);
    }
    child->m_dims.y = floorToInt(desiredYSize);
    delta += (desiredYSize - child->m_dims.y);

    if (delta > 1.0f)
    {
      const auto additionalSize = floorToInt(delta);
      child->m_dims.y += additionalSize;
      delta -= additionalSize;
    }

    child->m_pos.x = (m_dims.x - child->m_dims.x) / 2;
    child->m_pos.y = offset;

    offset += child->m_dims.y;
  }

  if (delta > 0.0f)
  {
    m_children.back()->m_dims.y += roundToInt(delta);
  }
}

} // namespace pge
