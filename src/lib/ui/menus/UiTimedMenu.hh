
#pragma once

#include "UiMenu.hh"
#include <core_utils/CoreObject.hh>
#include <core_utils/TimeUtils.hh>
#include <memory>

namespace pge {

struct TimedMenuConfig
{
  utils::Duration duration{utils::Milliseconds(1500)};

  bool fadeOut{true};
  utils::Duration fadeOutDuration{utils::Milliseconds(1000)};
  bool applyToBackGround{true};
};

class UiTimedMenu : public utils::CoreObject
{
  public:
  UiTimedMenu(UiMenuPtr menu);
  UiTimedMenu(const TimedMenuConfig &config, UiMenuPtr menu);
  ~UiTimedMenu() override = default;

  void trigger();
  void update();

  bool finished() const;

  void render(olc::PixelGameEngine *pge) const;

  private:
  UiMenuPtr m_menu{};
  utils::Duration m_duration{};
  std::optional<utils::Duration> m_fadeOutDuration{};
  bool m_applyToBackGround{true};

  std::optional<utils::TimeStamp> m_lastTrigger{};
  std::optional<utils::TimeStamp> m_fadeOutStartTime{};

  void initializeFromConfig(const TimedMenuConfig &config);
  void handleFadeOut(const utils::TimeStamp &now);
  void updateOpacity(const float perc);
};

using UiTimedMenuPtr = std::unique_ptr<UiTimedMenu>;

} // namespace pge
