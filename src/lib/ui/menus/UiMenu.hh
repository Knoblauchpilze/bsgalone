
#pragma once

#include "BackgroundConfig.hh"
#include "Controls.hh"
#include "MenuConfig.hh"
#include "TextConfig.hh"
#include "UserInputData.hh"
#include "olcEngine.hh"
#include <core_utils/CoreObject.hh>
#include <optional>

namespace pge {

class UiMenu;
using UiMenuPtr = std::unique_ptr<UiMenu>;

class UiMenu : public utils::CoreObject
{
  public:
  UiMenu(const MenuConfig &config, const BackgroundConfig &bg);
  UiMenu(const MenuConfig &config, const BackgroundConfig &bg, const TextConfig &text);
  ~UiMenu() override = default;

  bool visible() const noexcept;
  void setVisible(const bool visible) noexcept;

  auto tryGetText() const noexcept -> std::optional<std::string>;

  void addMenu(UiMenuPtr child);

  void render(olc::PixelGameEngine *pge) const;
  bool processUserInput(UserInputData &inputData);

  private:
  struct State
  {
    bool visible{true};

    bool higlightable{false};
    bool highlighted{false};

    bool propagateEventsToChildren{true};
  };

  olc::vf2d m_pos{};
  olc::vi2d m_dims{10, 10};

  State m_state{};
  MenuLayout m_layout{MenuLayout::Vertical};
  BackgroundConfig m_bg{};
  std::optional<TextConfig> m_text{};

  std::optional<HighlightCallback> m_highlightCallback{};
  std::optional<ClickCallback> m_clickCallback{};
  std::optional<GameCallback> m_gameClickCallback{};

  UiMenu *m_parent{nullptr};
  std::vector<UiMenuPtr> m_children{};

  void initializeFromConfig(const MenuConfig &config);
  auto absolutePosition() const noexcept -> olc::vi2d;
  void renderSelf(olc::PixelGameEngine *pge) const;
  void renderText(olc::PixelGameEngine *pge) const;
  auto getColorFromState() const -> olc::Pixel;
  auto getTextColorFromState() const -> olc::Pixel;
  bool isWithinMenu(const olc::vi2d &pos) const;
  void onRelevantInput(UserInputData &inputData);
  void updateLayoutAfterChildChange();
  void adaptChildrenToMatchHorizontalSize(const float desiredXSize);
  void adaptChildrenToMatchVerticalSize(const float desiredYSize);
};

} // namespace pge
