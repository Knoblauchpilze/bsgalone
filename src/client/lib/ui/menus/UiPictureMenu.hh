
#pragma once

#include "DecalResource.hh"
#include "UiMenu.hh"
#include <optional>

namespace pge {

struct PictureConfig
{
  std::optional<std::string> path{};
  std::optional<sprites::PackId> texturePack{};
  std::optional<Vec2i> spritePos{};

  Color tint{colors::WHITE};
};

class UiPictureMenu : public UiMenu
{
  public:
  UiPictureMenu(const MenuConfig &config, const PictureConfig &pictureConfig);
  ~UiPictureMenu() override = default;

  void setPictureTint(Color tint);
  void setSprite(const sprites::PackId texturePack, const Vec2i &spritePos);

  void clearSprite();

  protected:
  void renderCustom(Renderer &engine) const override;

  private:
  struct SpriteData
  {
    sprites::PackId texturePack{};
    Vec2i texturePos{};
  };

  DecalResourcePtr m_picture{};
  std::optional<SpriteData> m_sprite{};
  Color m_tint{colors::WHITE};

  void initializeFromConfig(const PictureConfig &config);

  void renderDecal(Renderer &renderer, const Vec2f &pos) const;
  void renderSprite(sprites::TexturePack &renderer, const Vec2f &pos) const;
};

using UiPictureMenuPtr = std::unique_ptr<UiPictureMenu>;

} // namespace pge
