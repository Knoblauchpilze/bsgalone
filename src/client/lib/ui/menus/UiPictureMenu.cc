
#include "UiPictureMenu.hh"
#include <unordered_set>

namespace pge {

UiPictureMenu::UiPictureMenu(const MenuConfig &config, const PictureConfig &pictureConfig)
  : UiMenu(config, BackgroundConfig{.color = colors::BLANK, .hColor = colors::BLANK})
  , m_tint(pictureConfig.tint)
{
  initializeFromConfig(pictureConfig);
}

void UiPictureMenu::setPictureTint(Color tint)
{
  m_tint = tint;
}

void UiPictureMenu::setSprite(const sprites::PackId texturePack, const Vec2i &spritePos)
{
  m_sprite = SpriteData{.texturePack = texturePack, .texturePos = spritePos};
}

void UiPictureMenu::clearSprite()
{
  m_sprite.reset();
}

void UiPictureMenu::renderCustom(Renderer &engine) const
{
  const auto iPos = absolutePosition();
  const Vec2f pos{static_cast<float>(iPos.x), static_cast<float>(iPos.y)};

  if (m_picture != nullptr)
  {
    renderDecal(engine, pos);
  }
  if (m_sprite)
  {
    renderSprite(engine.getTextureHandler(), pos);
  }
}

void UiPictureMenu::initializeFromConfig(const PictureConfig &config)
{
  if (!config.path && !config.texturePack)
  {
    error("Failed to initialize picture menu",
          "Expected at least one of path or texture pack to be provided");
  }

  if (config.path)
  {
    m_picture = std::make_unique<DecalResource>(*config.path);
  }
  if (config.texturePack)
  {
    SpriteData data{.texturePack = *config.texturePack};

    if (config.spritePos)
    {
      data.texturePos = *config.spritePos;
    }

    m_sprite = data;
  }
}

void UiPictureMenu::renderDecal(Renderer &renderer, const Vec2f &pos) const
{
  const auto size = dims();
  const Vec2f scale{1.0f * size.x / m_picture->w(), 1.0f * size.y / m_picture->h()};

  renderer.drawDecal(pos, *m_picture, scale, m_tint);
}

void UiPictureMenu::renderSprite(sprites::TexturePack &renderer, const Vec2f &pos) const
{
  const auto size = dims();
  const Vec2f scale{1.0f * size.x, 1.0f * size.y};

  sprites::Sprite sprite{.pack = m_sprite->texturePack, .sprite = m_sprite->texturePos};

  renderer.draw(sprite, pos, scale);
}

} // namespace pge
