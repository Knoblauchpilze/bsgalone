
#pragma once

#include "CoreObject.hh"
#include "TimeUtils.hh"
#include "UiMenu.hh"
#include <memory>

namespace pge {

struct BlinkingMenuConfig
{
  core::Duration blinkingDuration{core::Milliseconds(1500)};

  bool applyToBackground{true};
  bool applyToText{true};
};

class UiBlinkingMenu : public core::CoreObject
{
  public:
  UiBlinkingMenu(UiMenuPtr menu);
  UiBlinkingMenu(const BlinkingMenuConfig &config, UiMenuPtr menu);
  ~UiBlinkingMenu() override = default;

  void setVisible(const bool visible);
  void update();

  void render(Renderer &engine) const;

  private:
  UiMenuPtr m_menu{};
  core::Duration m_blinkingDuration{};
  bool m_applyToBackground{true};
  bool m_applyToText{true};

  enum class State
  {
    FADE_IN,
    FADE_OUT
  };

  State m_blinkingState{State::FADE_OUT};
  std::optional<core::TimeStamp> m_lastTrigger{};

  void initializeFromConfig(const BlinkingMenuConfig &config);
  void handleBlinking(const core::TimeStamp &now);
  void updateOpacity(const float perc);
};

using UiBlinkingMenuPtr = std::unique_ptr<UiBlinkingMenu>;

} // namespace pge
