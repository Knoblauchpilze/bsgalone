
#pragma once

#include "DecalResource.hh"
#include "UiMenu.hh"
#include <optional>

namespace ui {

struct PictureConfig
{
  std::optional<std::string> path{};
  std::optional<pge::sprites::PackId> texturePack{};
  std::optional<pge::Vec2i> spritePos{};

  pge::Color tint{pge::colors::WHITE};
};

class UiPictureMenu : public UiMenu
{
  public:
  UiPictureMenu(const MenuConfig &config, const PictureConfig &pictureConfig);
  ~UiPictureMenu() override = default;

  void setPictureTint(pge::Color tint);
  void setSprite(const pge::sprites::PackId texturePack, const pge::Vec2i &spritePos);

  void clearSprite();

  protected:
  void renderCustom(pge::Renderer &engine) const override;

  private:
  struct SpriteData
  {
    pge::sprites::PackId texturePack{};
    pge::Vec2i texturePos{};
  };

  pge::DecalResourcePtr m_picture{};
  std::optional<SpriteData> m_sprite{};
  pge::Color m_tint{pge::colors::WHITE};

  void initializeFromConfig(const PictureConfig &config);

  void renderDecal(pge::Renderer &renderer, const pge::Vec2f &pos) const;
  void renderSprite(pge::sprites::TexturePack &renderer, const pge::Vec2f &pos) const;
};

using UiPictureMenuPtr = std::unique_ptr<UiPictureMenu>;

} // namespace ui
