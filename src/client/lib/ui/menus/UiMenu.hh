
#pragma once

#include "BackgroundConfig.hh"
#include "Controls.hh"
#include "MenuConfig.hh"
#include "olcEngine.hh"
#include <core_utils/CoreObject.hh>
#include <memory>
#include <optional>

namespace pge {

class UiMenu;
using UiMenuPtr = std::unique_ptr<UiMenu>;

class UiMenu : public utils::CoreObject
{
  public:
  UiMenu(const MenuConfig &config, const BackgroundConfig &bg);
  ~UiMenu() override = default;

  bool visible() const noexcept;
  void setVisible(const bool visible) noexcept;
  void setEnabled(const bool enabled) noexcept;
  void setHighlightable(const bool highlightable) noexcept;
  void setPosition(const olc::vi2d &position);

  void setClickCallback(const ClickCallback &callback);
  void setGameClickCallback(const GameCallback &callback);

  void addMenu(UiMenuPtr child);
  void clearChildren();
  auto getBgColor() const -> olc::Pixel;
  void updateBgColor(const olc::Pixel &color);

  void render(olc::PixelGameEngine *pge) const;
  virtual bool processUserInput(UserInputData &inputData);

  protected:
  auto absolutePosition() const noexcept -> olc::vi2d;
  auto dims() const noexcept -> olc::vi2d;

  struct State
  {
    bool visible{true};

    bool disabled{false};
    bool highlightable{false};
    bool highlighted{false};

    bool propagateEventsToChildren{true};
  };

  auto state() const noexcept -> const State &;
  void setLostFocusCallback(const ClickCallback &callback);

  virtual void renderCustom(olc::PixelGameEngine *pge) const;

  private:
  olc::vf2d m_pos{};
  olc::vi2d m_dims{10, 10};

  State m_state{};
  MenuLayout m_layout{MenuLayout::VERTICAL};
  bool m_expandChildrenToFit{true};
  BackgroundConfig m_bg{};

  std::optional<HighlightCallback> m_highlightCallback{};
  std::optional<ClickCallback> m_clickCallback{};
  std::optional<LostFocusCallback> m_lostFocusCallback{};
  std::optional<GameCallback> m_gameClickCallback{};

  UiMenu *m_parent{nullptr};
  std::vector<UiMenuPtr> m_children{};

  void initializeFromConfig(const MenuConfig &config);
  void renderSelf(olc::PixelGameEngine *pge) const;
  auto getColorFromState() const -> olc::Pixel;
  bool isWithinMenu(const olc::vi2d &pos) const;
  void onRelevantInput(UserInputData &inputData);
  void updateLayoutAfterChildChange();
  void adaptChildrenToMatchHorizontalSize(const float desiredXSize);
  void adaptChildrenToMatchVerticalSize(const float desiredYSize);
};

} // namespace pge