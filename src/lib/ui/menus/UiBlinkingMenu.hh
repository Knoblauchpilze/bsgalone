
#pragma once

#include "UiMenu.hh"
#include <core_utils/CoreObject.hh>
#include <core_utils/TimeUtils.hh>
#include <memory>

namespace pge {

struct BlinkingMenuConfig
{
  utils::Duration blinkingDuration{utils::Milliseconds(1500)};

  bool applyToBackground{true};
  bool applyToText{true};
};

class UiBlinkingMenu : public utils::CoreObject
{
  public:
  UiBlinkingMenu(UiMenuPtr menu);
  UiBlinkingMenu(const BlinkingMenuConfig &config, UiMenuPtr menu);
  ~UiBlinkingMenu() override = default;

  void setVisible(const bool visible);
  void update();

  void render(olc::PixelGameEngine *pge) const;

  private:
  UiMenuPtr m_menu{};
  utils::Duration m_blinkingDuration{};
  bool m_applyToBackground{true};
  bool m_applyToText{true};

  enum class State
  {
    FADE_IN,
    FADE_OUT
  };

  State m_blinkingState{State::FADE_OUT};
  std::optional<utils::TimeStamp> m_lastTrigger{};

  void initializeFromConfig(const BlinkingMenuConfig &config);
  void handleBlinking(const utils::TimeStamp &now);
  void updateOpacity(const float perc);
};

using UiBlinkingMenuPtr = std::unique_ptr<UiBlinkingMenu>;

} // namespace pge
