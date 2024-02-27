
#pragma once

#include "DecalResource.hh"
#include "UiMenu.hh"

namespace pge {

struct PictureConfig
{
  std::string path{};
  Color tint{colors::WHITE};
};

class UiPictureMenu : public UiMenu
{
  public:
  UiPictureMenu(const MenuConfig &config, const PictureConfig &pictureConfig);
  ~UiPictureMenu() override = default;

  void setPictureTint(Color tint);

  protected:
  void renderCustom(Renderer &engine) const override;

  private:
  DecalResourcePtr m_picture{};
  Color m_tint{colors::WHITE};
};

using UiPictureMenuPtr = std::unique_ptr<UiPictureMenu>;

} // namespace pge
