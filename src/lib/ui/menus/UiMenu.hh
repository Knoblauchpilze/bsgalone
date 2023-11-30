
#pragma once

#include "BackgroundConfig.hh"
#include "Controls.hh"
#include "MenuConfig.hh"
#include "UserInputData.hh"
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

  void addMenu(UiMenuPtr child);
  void updateBgColor(const olc::Pixel &color);

  void render(olc::PixelGameEngine *pge) const;
  virtual bool processUserInput(UserInputData &inputData);

  protected:
  auto absolutePosition() const noexcept -> olc::vi2d;
  auto dims() const noexcept -> olc::vi2d;

  struct State
  {
    bool visible{true};

    bool higlightable{false};
    bool highlighted{false};

    bool propagateEventsToChildren{true};
  };

  auto state() const noexcept -> const State &;
  void setClickCallback(const ClickCallback &callback);
  void setLostFocusCallback(const ClickCallback &callback);

  virtual void renderCustom(olc::PixelGameEngine *pge) const;

  private:
  olc::vf2d m_pos{};
  olc::vi2d m_dims{10, 10};

  State m_state{};
  MenuLayout m_layout{MenuLayout::Vertical};
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
