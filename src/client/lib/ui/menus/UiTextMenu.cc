
#include "UiTextMenu.hh"
#include "VectorConversion.hh"

namespace pge {

UiTextMenu::UiTextMenu(const MenuConfig &config, const BackgroundConfig &bg, const TextConfig &text)
  : UiMenu(config, bg)
  , m_text(text)
{
  addModule("text");
}

auto UiTextMenu::getText() const noexcept -> std::string
{
  return m_text.text;
}

void UiTextMenu::setText(const std::string &text)
{
  m_text.text = text;
}

auto UiTextMenu::getTextColor() const -> olc::Pixel
{
  return m_text.color;
}

void UiTextMenu::updateTextColor(const olc::Pixel &color)
{
  m_text = textConfigFromColor(m_text.text, color, m_text.align);
}

namespace {
auto computeTextPositionFromAlignement(const Vec2i &offset,
                                       const Vec2i &dims,
                                       const Vec2i &textDims,
                                       const TextAlignment &align) -> Vec2i
{
  Vec2i textPos{};
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

void UiTextMenu::renderCustom(Renderer &engine) const
{
  const auto absPos   = absolutePosition();
  const auto textDims = engine.getTextSize(m_text.text);
  const auto textPos  = computeTextPositionFromAlignement(absPos, dims(), textDims, m_text.align);
  const auto color    = getTextColorFromState();

  engine.drawString(toVec2f(textPos), m_text.text, color);
}

auto UiTextMenu::getTextColorFromState() const -> olc::Pixel
{
  if (state().highlightable && state().highlighted)
  {
    return m_text.hColor;
  }
  return m_text.color;
}

} // namespace pge
