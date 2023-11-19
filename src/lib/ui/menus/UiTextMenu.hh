
#pragma once

#include "TextConfig.hh"
#include "UiMenu.hh"

namespace pge {

class UiTextMenu : public UiMenu
{
  public:
  UiTextMenu(const MenuConfig &config, const BackgroundConfig &bg, const TextConfig &text);
  ~UiTextMenu() override = default;

  auto getText() const noexcept -> std::string;

  protected:
  void renderCustom(olc::PixelGameEngine *pge) const override;

  private:
  TextConfig m_text{};

  auto getTextColorFromState() const -> olc::Pixel;
};

using UiTextMenuPtr = std::unique_ptr<UiTextMenu>;

} // namespace pge
