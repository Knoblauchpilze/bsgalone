
#pragma once

#include "TextConfig.hh"
#include "UiMenu.hh"

namespace ui {

class UiTextMenu : public UiMenu
{
  public:
  UiTextMenu(const MenuConfig &config, const BackgroundConfig &bg, const TextConfig &text);
  ~UiTextMenu() override = default;

  virtual auto getText() const noexcept -> std::string;
  void setText(const std::string &text);
  auto getTextColor() const -> pge::Color;
  void updateTextColor(const pge::Color &color);

  protected:
  void renderCustom(pge::Renderer &engine) const override;

  private:
  TextConfig m_text{};

  auto getTextColorFromState() const -> pge::Color;
};

using UiTextMenuPtr = std::unique_ptr<UiTextMenu>;

} // namespace ui
