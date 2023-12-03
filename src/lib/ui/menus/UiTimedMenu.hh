
#pragma once

#include "UiMenu.hh"
#include <core_utils/CoreObject.hh>
#include <core_utils/TimeUtils.hh>

namespace pge {

class UiTimedMenu : public utils::CoreObject
{
  UiTimedMenu(const utils::Duration &duration, UiMenuPtr menu);
  ~UiTimedMenu() override = default;

  void trigger();
  void update(const float elapsedSeconds);

  private:
  UiMenuPtr m_menu{};
  utils::Duration m_duration{};

  std::optional<utils::Duration> m_elapsedSinceLastTrigger{};
};

} // namespace pge
