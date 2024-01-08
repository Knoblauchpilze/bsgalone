
#include "UiBlinkingMenu.hh"
#include "UiTextMenu.hh"

namespace pge {

UiBlinkingMenu::UiBlinkingMenu(UiMenuPtr menu)
  : UiBlinkingMenu(BlinkingMenuConfig{}, std::move(menu))
{}

UiBlinkingMenu::UiBlinkingMenu(const BlinkingMenuConfig &config, UiMenuPtr menu)
  : utils::CoreObject("blinking")
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

void UiBlinkingMenu::setVisible(const bool visible)
{
  const auto shouldContinueBlinking = visible && m_lastTrigger;
  const auto shouldNotStartBlinking = !m_lastTrigger && !visible;
  if (shouldContinueBlinking || shouldNotStartBlinking)
  {
    return;
  }

  m_menu->setVisible(visible);
  updateOpacity(1.0f);

  if (!visible)
  {
    m_lastTrigger.reset();
    return;
  }

  m_lastTrigger   = utils::now();
  m_blinkingState = State::FADE_OUT;
}

void UiBlinkingMenu::update()
{
  if (!m_lastTrigger)
  {
    return;
  }

  const auto now     = utils::now();
  const auto elapsed = now - *m_lastTrigger;
  handleBlinking(now);

  if (elapsed >= m_blinkingDuration)
  {
    m_lastTrigger   = now;
    m_blinkingState = (State::FADE_OUT == m_blinkingState ? State::FADE_IN : State::FADE_OUT);
  }
}

void UiBlinkingMenu::render(Renderer &engine) const
{
  m_menu->render(engine);
}

void UiBlinkingMenu::initializeFromConfig(const BlinkingMenuConfig &config)
{
  m_blinkingDuration = config.blinkingDuration;

  m_applyToBackground = config.applyToBackground;
  m_applyToText       = config.applyToText;
}

void UiBlinkingMenu::handleBlinking(const utils::TimeStamp &now)
{
  const auto fadeElapsedTimeAsFloat = std::chrono::duration<float, std::milli>(now - *m_lastTrigger);

  auto perc = 1.0f - fadeElapsedTimeAsFloat / m_blinkingDuration;
  if (State::FADE_IN == m_blinkingState)
  {
    perc = 1.0f - perc;
  }

  updateOpacity(perc);
}

void UiBlinkingMenu::updateOpacity(const float perc)
{
  if (m_applyToBackground)
  {
    const auto bg = makeTransparentFromPercentage(m_menu->getBgColor(), perc);
    m_menu->updateBgColor(bg);
  }

  if (m_applyToText)
  {
    const auto maybeTextMenu = dynamic_cast<UiTextMenu *>(m_menu.get());
    if (nullptr != maybeTextMenu)
    {
      const auto text = makeTransparentFromPercentage(maybeTextMenu->getTextColor(), perc);
      maybeTextMenu->updateTextColor(text);
    }
  }
}

} // namespace pge
