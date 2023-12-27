
#include "UiTimedMenu.hh"
#include "UiTextMenu.hh"

namespace pge {

UiTimedMenu::UiTimedMenu(UiMenuPtr menu)
  : UiTimedMenu(TimedMenuConfig{}, std::move(menu))
{}

UiTimedMenu::UiTimedMenu(const TimedMenuConfig &config, UiMenuPtr menu)
  : utils::CoreObject("timed")
  , m_menu(std::move(menu))
{
  setService("menu");

  if (nullptr == m_menu)
  {
    throw std::invalid_argument("Expected non null menu");
  }

  initializeFromConfig(config);
  m_menu->setVisible(false);
}

void UiTimedMenu::trigger()
{
  m_menu->setVisible(true);
  updateOpacity(1.0f);

  m_lastTrigger = utils::now();
  if (m_fadeOutDuration)
  {
    m_fadeOutStartTime = *m_lastTrigger + m_duration - *m_fadeOutDuration;
  }
}

void UiTimedMenu::update()
{
  if (!m_lastTrigger)
  {
    return;
  }

  const auto now     = utils::now();
  const auto elapsed = now - *m_lastTrigger;
  handleFadeOut(now);

  if (elapsed >= m_duration)
  {
    m_lastTrigger.reset();
    m_fadeOutStartTime.reset();

    m_menu->setVisible(false);
  }
}

bool UiTimedMenu::finished() const
{
  return m_lastTrigger.has_value();
}

void UiTimedMenu::render(olc::PixelGameEngine *pge) const
{
  m_menu->render(pge);
}

void UiTimedMenu::initializeFromConfig(const TimedMenuConfig &config)
{
  m_duration = config.duration;

  if (!config.fadeOut)
  {
    return;
  }

  m_fadeOutDuration = config.fadeOutDuration;
}

void UiTimedMenu::handleFadeOut(const utils::TimeStamp &now)
{
  if (!m_fadeOutStartTime || now < *m_fadeOutStartTime)
  {
    return;
  }

  const auto fadeOutElapsedTimeAsFloat = std::chrono::duration<float, std::milli>(
    now - *m_fadeOutStartTime);
  const auto perc = 1.0f - fadeOutElapsedTimeAsFloat / *m_fadeOutDuration;
  updateOpacity(perc);
}

void UiTimedMenu::updateOpacity(const float perc)
{
  const auto bg = opacifyFromPercentage(m_menu->getBgColor(), perc);
  m_menu->updateBgColor(bg);

  const auto maybeTextMenu = dynamic_cast<UiTextMenu *>(m_menu.get());
  if (nullptr != maybeTextMenu)
  {
    const auto text = opacifyFromPercentage(maybeTextMenu->getTextColor(), perc);
    maybeTextMenu->updateTextColor(text);
  }
}

} // namespace pge
