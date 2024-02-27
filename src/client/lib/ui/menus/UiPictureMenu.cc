
#include "UiPictureMenu.hh"
#include <unordered_set>

namespace pge {

UiPictureMenu::UiPictureMenu(const MenuConfig &config, const PictureConfig &pictureConfig)
  : UiMenu(config, BackgroundConfig{.color = colors::BLANK, .hColor = colors::BLANK})
  , m_picture(std::make_unique<DecalResource>(pictureConfig.path))
  , m_tint(pictureConfig.tint)
{}

void UiPictureMenu::setPictureTint(Color tint)
{
  m_tint = tint;
}

void UiPictureMenu::renderCustom(Renderer &engine) const
{
  const auto iPos = absolutePosition();
  const Vec2f pos{static_cast<float>(iPos.x), static_cast<float>(iPos.y)};

  const auto size = dims();
  const Vec2f scale{1.0f * size.x / m_picture->w(), 1.0f * size.y / m_picture->h()};

  engine.drawDecal(pos, *m_picture, scale, m_tint);
}

} // namespace pge
