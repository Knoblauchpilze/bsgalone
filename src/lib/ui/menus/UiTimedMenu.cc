
#include "UiTimedMenu.hh"

namespace pge {

UiTimedMenu::UiTimedMenu(const utils::Duration &duration, UiMenuPtr menu)
  : utils::CoreObject("timed")
  , m_menu(std::move(menu))
  , m_duration(duration)
{
  setService("menu");

  if (nullptr == m_menu)
  {
    throw std::invalid_argument("Expected non null menu");
  }
}

void UiTimedMenu::trigger()
{
  m_elapsedSinceLastTrigger = utils::Duration(0);
  m_menu->setVisible(true);
}

void UiTimedMenu::update(const float elapsedSeconds)
{
  if (!m_elapsedSinceLastTrigger)
  {
    return;
  }

  constexpr auto MILLISECONDS_IN_A_SECONDS = 1000;
  (*m_elapsedSinceLastTrigger) += utils::Milliseconds(
    static_cast<int>(elapsedSeconds * MILLISECONDS_IN_A_SECONDS));

  if (*m_elapsedSinceLastTrigger >= m_duration)
  {
    m_elapsedSinceLastTrigger.reset();
    m_menu->setVisible(false);
  }
}

} // namespace pge
