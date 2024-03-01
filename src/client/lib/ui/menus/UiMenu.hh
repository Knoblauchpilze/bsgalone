
#pragma once

#include "BackgroundConfig.hh"
#include "Controls.hh"
#include "MenuConfig.hh"
#include "Renderer.hh"
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
  void setPosition(const Vec2i &position);

  void setClickCallback(const std::optional<ClickCallback> &callback);
  void setGameClickCallback(const std::optional<GameCallback> &callback);

  void addMenu(UiMenuPtr child);
  void clearChildren();
  auto getBgColor() const -> Color;
  void updateBgColor(const Color &color);

  void render(Renderer &engine) const;
  virtual bool processUserInput(UserInputData &inputData);

  protected:
  auto absolutePosition() const noexcept -> Vec2i;
  auto dims() const noexcept -> Vec2i;

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

  virtual void renderCustom(Renderer &engine) const;

  private:
  Vec2i m_pos{};
  Vec2i m_dims{10, 10};

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
  void renderSelf(Renderer &engine) const;
  auto getColorFromState() const -> Color;
  bool isWithinMenu(const Vec2i &pos) const;
  void onRelevantInput(UserInputData &inputData);
  void updateLayoutAfterChildChange();
  void adaptChildrenToMatchHorizontalSize(const float desiredXSize);
  void adaptChildrenToMatchVerticalSize(const float desiredYSize);
};

} // namespace pge
