
#include "UiMenu.hh"

namespace pge {

UiMenu::UiMenu(const MenuConfig &config, const BackgroundConfig &bg)
  : utils::CoreObject("menu")
  , m_bg(bg)
{
  initializeFromConfig(config);
}

UiMenu::UiMenu(const MenuConfig &config, const BackgroundConfig &bg, const TextConfig &text)
  : utils::CoreObject("menu")
  , m_bg(bg)
  , m_text(text)
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

auto UiMenu::tryGetText() const noexcept -> std::optional<std::string>
{
  if (!m_text)
  {
    return {};
  }

  return m_text->text;
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
    return inputRelevantForChildren;
  }

  onRelevantInput(inputData);

  return true;
}

void UiMenu::initializeFromConfig(const MenuConfig &config)
{
  m_pos  = config.pos;
  m_dims = config.dims;

  m_layout = config.layout;

  m_state.visible                   = config.visible;
  m_state.higlightable              = config.highligtable;
  m_state.propagateEventsToChildren = config.propagateEventsToChildren;

  m_highlightCallback = config.highlightCallback;
  m_clickCallback     = config.clickCallback;
  m_gameClickCallback = config.gameClickCallBack;
}

inline auto UiMenu::absolutePosition() const noexcept -> olc::vi2d
{
  auto p = m_pos;
  if (m_parent != nullptr)
  {
    p += m_parent->absolutePosition();
  }
  return p;
}

void UiMenu::renderSelf(olc::PixelGameEngine *pge) const
{
  const auto absPos = absolutePosition();
  const auto color  = getColorFromState();
  pge->FillRectDecal(absPos, m_dims, color);
  renderText(pge);
}

namespace {
auto computeTextPositionFromAlignement(const olc::vi2d &offset,
                                       const olc::vi2d &dims,
                                       const olc::vi2d &textDims,
                                       const TextAlignment &align) -> olc::vi2d
{
  olc::vi2d textPos{};
  switch (align)
  {
    case TextAlignment::CENTER:
      textPos.x = static_cast<int>(offset.x + (dims.x - textDims.x) / 2.0f);
      textPos.y = static_cast<int>(offset.y + (dims.y - textDims.y) / 2.0f);
      break;
    case TextAlignment::RIGHT:
      textPos.x = offset.x + dims.x - textDims.x;
      textPos.y = static_cast<int>(offset.y + (dims.y - textDims.y) / 2.0f);
      break;
    case TextAlignment::LEFT:
    default:
      textPos.x = offset.x;
      textPos.y = static_cast<int>(offset.y + (dims.y - textDims.y) / 2.0f);
      break;
  }

  return textPos;
}
} // namespace

void UiMenu::renderText(olc::PixelGameEngine *pge) const
{
  if (!m_text)
  {
    return;
  }

  const auto absPos   = absolutePosition();
  const auto textDims = pge->GetTextSize(m_text->text);
  const auto textPos  = computeTextPositionFromAlignement(absPos, m_dims, textDims, m_text->align);
  const auto color    = getTextColorFromState();

  pge->DrawStringDecal(textPos, m_text->text, color);
}

auto UiMenu::getColorFromState() const -> olc::Pixel
{
  if (m_state.higlightable && m_state.highlighted)
  {
    return m_bg.hColor;
  }
  return m_bg.color;
}

auto UiMenu::getTextColorFromState() const -> olc::Pixel
{
  if (m_state.higlightable && m_state.highlighted)
  {
    return m_text->hColor;
  }
  return m_text->color;
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

  const auto userClicked = (controls::ButtonState::RELEASED
                            == inputData.controls.buttons[controls::mouse::LEFT]);
  if (userClicked)
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
  const auto sizeToDistribute = (m_layout == MenuLayout::Horizontal ? m_dims.x : m_dims.y);
  const auto count            = static_cast<int>(m_children.size());
  const auto sizeForEachChild = 1.0f * sizeToDistribute / count;

  switch (m_layout)
  {
    case MenuLayout::Horizontal:
      adaptChildrenToMatchHorizontalSize(sizeForEachChild);
      break;
    case MenuLayout::Vertical:
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
    child->m_dims.y = std::min(m_dims.y, child->m_dims.y);

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
    child->m_dims.x = std::min(m_dims.x, child->m_dims.x);
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
