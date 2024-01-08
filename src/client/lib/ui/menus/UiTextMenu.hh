
#pragma once

#include "TextConfig.hh"
#include "UiMenu.hh"

namespace pge {

class UiTextMenu : public UiMenu
{
  public:
  UiTextMenu(const MenuConfig &config, const BackgroundConfig &bg, const TextConfig &text);
  ~UiTextMenu() override = default;

  virtual auto getText() const noexcept -> std::string;
  void setText(const std::string &text);
  auto getTextColor() const -> Color;
  void updateTextColor(const Color &color);

  protected:
  void renderCustom(Renderer &engine) const override;

  private:
  TextConfig m_text{};

  auto getTextColorFromState() const -> Color;
};

using UiTextMenuPtr = std::unique_ptr<UiTextMenu>;

} // namespace pge
