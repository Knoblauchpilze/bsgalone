
#include "LogUiHandler.hh"
#include "UiTextMenu.hh"

namespace pge {

LogUiHandler::LogUiHandler(const bsgo::Views & /*views*/)
  : IUiHandler("log")
  , AbstractMessageListener({bsgo::MessageType::SYSTEM})
{}

void LogUiHandler::initializeMenus(const int width, const int height)
{
  m_offset = olc::vi2d{width / 2, height / 4};
}

bool LogUiHandler::processUserInput(UserInputData & /*inputData*/)
{
  // Timed menus can't process user input.
  return false;
}

void LogUiHandler::render(SpriteRenderer &engine) const
{
  for (const auto &menu : m_logs)
  {
    menu->render(engine.getRenderer());
  }
}

void LogUiHandler::updateUi()
{
  for (const auto &menu : m_logs)
  {
    menu->update();
  }

  std::erase_if(m_logs, [](const UiTimedMenuPtr &menu) { return menu->finished(); });
}

void LogUiHandler::reset()
{
  m_logs.clear();
}

void LogUiHandler::connectToMessageQueue(bsgo::IMessageQueue &messageQueue)
{
  messageQueue.addListener(this);
}

namespace {
constexpr std::size_t MAXIMUM_NUMBER_OF_LOGS_DISPLAYED = 5;
} // namespace

void LogUiHandler::onMessageReceived(const bsgo::IMessage &message)
{
  if (m_logs.size() > MAXIMUM_NUMBER_OF_LOGS_DISPLAYED)
  {
    m_logs.pop_back();
  }

  auto logMenu = createMenuFromMessage(message);
  logMenu->trigger();
  /// TODO: Should push the menus down the screen
  m_logs.push_front(std::move(logMenu));
}

namespace {
constexpr auto LOG_FADE_OUT_DURATION_MS = 5000;

const auto LOG_MENU_DIMS = olc::vi2d{150, 30};
} // namespace

auto LogUiHandler::createMenuFromMessage(const bsgo::IMessage & /*message*/) -> UiTimedMenuPtr
{
  const olc::vi2d pos{m_offset.x - LOG_MENU_DIMS.x / 2, m_offset.y};
  const MenuConfig config{.pos = pos, .dims = LOG_MENU_DIMS, .highlightable = false};
  const BackgroundConfig bg = bgConfigFromColor(transparent(olc::WHITE, alpha::Transparent));
  const TextConfig text     = textConfigFromColor("Message", olc::WHITE);

  auto menu = std::make_unique<UiTextMenu>(config, bg, text);

  TimedMenuConfig timedConfig{.duration          = utils::Milliseconds(LOG_FADE_OUT_DURATION_MS),
                              .fadeOut           = true,
                              .fadeOutDuration   = utils::Milliseconds(LOG_FADE_OUT_DURATION_MS),
                              .applyToBackGround = false};
  return std::make_unique<UiTimedMenu>(timedConfig, std::move(menu));
}

} // namespace pge
