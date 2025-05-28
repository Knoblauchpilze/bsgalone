
#pragma once

#include "CoreObject.hh"
#include "TimeUtils.hh"
#include "UiMenu.hh"
#include <memory>

namespace pge {

struct TimedMenuConfig
{
  core::Duration duration{core::Milliseconds(1500)};

  bool fadeOut{true};
  core::Duration fadeOutDuration{core::Milliseconds(1000)};
  bool applyToBackGround{true};
};

class UiTimedMenu : public core::CoreObject
{
  public:
  UiTimedMenu(UiMenuPtr menu);
  UiTimedMenu(const TimedMenuConfig &config, UiMenuPtr menu);
  ~UiTimedMenu() override = default;

  void trigger();
  void update();

  bool finished() const;

  void render(Renderer &engine) const;

  private:
  UiMenuPtr m_menu{};
  core::Duration m_duration{};
  std::optional<core::Duration> m_fadeOutDuration{};
  bool m_applyToBackGround{true};

  std::optional<core::TimeStamp> m_lastTrigger{};
  std::optional<core::TimeStamp> m_fadeOutStartTime{};

  void initializeFromConfig(const TimedMenuConfig &config);
  void handleFadeOut(const core::TimeStamp &now);
  void updateOpacity(const float perc);
};

using UiTimedMenuPtr = std::unique_ptr<UiTimedMenu>;

} // namespace pge
