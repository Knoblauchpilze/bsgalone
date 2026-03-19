
#include "LogUiHandler.hh"
#include "GameColorUtils.hh"
#include "IViewListenerProxy.hh"
#include "MessageListenerWrapper.hh"
#include "StringUtils.hh"
#include "UiTextMenu.hh"

#include "JumpCancelledMessage.hh"
#include "JumpRequestedMessage.hh"
#include "LootMessage.hh"
#include "ScannedMessage.hh"
#include "SlotComponentMessage.hh"

namespace bsgalone::client {

const std::unordered_set<core::MessageType> RELEVANT_MESSAGE_TYPES_TO_LOG
  = {core::MessageType::JUMP_CANCELLED,
     core::MessageType::JUMP_REQUESTED,
     core::MessageType::LOOT,
     core::MessageType::SCANNED,
     core::MessageType::SLOT_COMPONENT_UPDATED};

LogUiHandler::LogUiHandler(const Views &views)
  : AbstractUiHandler("log")
  , AbstractMessageListener(RELEVANT_MESSAGE_TYPES_TO_LOG)
  , m_gameView(views.gameView)
{
  if (nullptr == m_gameView)
  {
    throw std::invalid_argument("Expected non null game view");
  }

  subscribeToViews();
}

void LogUiHandler::initializeMenus(const int width,
                                   const int height,
                                   pge::sprites::TexturePack & /*texturesLoader*/)
{
  m_offset = pge::Vec2i{width / 2, height / 4};
}

bool LogUiHandler::processUserInput(ui::UserInputData & /*inputData*/)
{
  // Timed menus can't process user input.
  return false;
}

void LogUiHandler::render(pge::Renderer &engine) const
{
  for (const auto &message : m_logs)
  {
    message.menu->render(engine);
  }
}

void LogUiHandler::updateUi()
{
  for (const auto &menu : m_logsToTrigger)
  {
    menu->trigger();
  }
  m_logsToTrigger.clear();

  for (const auto &message : m_logs)
  {
    message.menu->update();
  }

  std::erase_if(m_logs, [](const LogMessage &data) { return data.menu->finished(); });
}

void LogUiHandler::connectToMessageQueue(core::IMessageQueue &messageQueue)
{
  auto listener = std::make_unique<MessageListenerWrapper>(this);
  messageQueue.addListener(std::move(listener));
}

namespace {
constexpr std::size_t MAXIMUM_NUMBER_OF_LOGS_DISPLAYED = 5;
const pge::Vec2i LOG_MENU_DIMS{150, 20};
constexpr auto LOG_FADE_OUT_DURATION_MS = 7000;

bool shouldSlotComponentUpdatedMessageBeFiltered(const core::SlotComponentMessage &message,
                                                 const core::Entity &playerShip)
{
  switch (message.getComponentType())
  {
    case core::ComponentType::COMPUTER_SLOT:
      return (*playerShip.tryGetComputer(message.getSlotDbId()))->isOffensive();
    default:
      return true;
  }
}

bool shouldMessageBeFiltered(const core::IMessage &message, const core::Entity &playerShip)
{
  switch (message.type())
  {
    case core::MessageType::SLOT_COMPONENT_UPDATED:
      return shouldSlotComponentUpdatedMessageBeFiltered(message.as<core::SlotComponentMessage>(),
                                                         playerShip);
    default:
      return false;
  }
}
} // namespace

void LogUiHandler::onEventReceived(const core::IMessage &message)
{
  if (!m_gameView->isReady())
  {
    return;
  }

  if (m_logs.size() > MAXIMUM_NUMBER_OF_LOGS_DISPLAYED)
  {
    m_logs.pop_back();
  }

  const auto ship = m_gameView->getPlayerShip();
  if (shouldMessageBeFiltered(message, ship))
  {
    return;
  }

  auto logMenu = createMenuFromMessage(message);
  LogMessage data{};
  data.rawMenu = logMenu.get();

  ui::TimedMenuConfig timedConfig{.duration        = ::core::Milliseconds(LOG_FADE_OUT_DURATION_MS),
                                  .fadeOut         = true,
                                  .fadeOutDuration = ::core::Milliseconds(LOG_FADE_OUT_DURATION_MS),
                                  .applyToBackGround = false};
  data.menu = std::make_unique<ui::UiTimedMenu>(timedConfig, std::move(logMenu));

  auto id = 1;
  for (const auto &data : m_logs)
  {
    const auto y = m_offset.y + id * LOG_MENU_DIMS.y;
    const pge::Vec2i pos{m_offset.x - LOG_MENU_DIMS.x / 2, y};
    data.rawMenu->setPosition(pos);
    ++id;
  }

  m_logsToTrigger.push_back(data.menu.get());
  m_logs.emplace_front(std::move(data));
}

namespace {
constexpr auto FTL_JUMP_STARTED_TEXT = "FTL jump sequence started";

auto createJumpRequestedMessage(const core::JumpRequestedMessage & /*message*/)
{
  return ui::textConfigFromColor(FTL_JUMP_STARTED_TEXT, pge::colors::WHITE);
}

constexpr auto FTL_JUMP_CANCELLED_TEXT = "FTL jump sequence aborted";

auto createJumpCancelledMessage(const core::JumpCancelledMessage & /*message*/)
{
  return ui::textConfigFromColor(FTL_JUMP_CANCELLED_TEXT, pge::colors::WHITE);
}

auto createLootMessage(const core::LootMessage &message, const GameView &gameView) -> ui::TextConfig
{
  const auto resource = gameView.getResourceName(message.getResourceDbId());
  const auto text     = "+" + std::to_string(message.amount()) + " " + resource;
  const auto color    = colorFromResourceName(resource);
  return ui::textConfigFromColor(text, color);
}

constexpr auto NO_USEFUL_RESOURCES_TEXT = "Mineral analysis: no useful resources";

auto createScannedMessage(const core::ScannedMessage &message, const GameView &gameView)
  -> ui::TextConfig
{
  const auto asteroid = gameView.getAsteroid(message.getAsteroidDbId());

  if (!asteroid.exists<core::LootComponent>())
  {
    return ui::textConfigFromColor(NO_USEFUL_RESOURCES_TEXT, pge::colors::WHITE);
  }

  if (asteroid.resources.size() > 1)
  {
    throw std::invalid_argument("Expected asteroid to contain only one resource as loot but got "
                                + std::to_string(asteroid.resources.size()));
  }
  const auto loot = asteroid.resources.at(0);

  const auto resource = gameView.getResourceName(loot->resource());
  const auto text     = "Mineral analysis: " + std::to_string(loot->amount()) + " " + resource;
  const auto color    = colorFromResourceName(resource);
  return ui::textConfigFromColor(text, color);
}

constexpr auto ELECTRONIC_SUPPORT_TEXT = "Your systems are being improved by electronic support";

auto createSlotMessage(const core::SlotComponentMessage & /*message*/) -> ui::TextConfig
{
  return ui::textConfigFromColor(ELECTRONIC_SUPPORT_TEXT, pge::colors::APPLE_GREEN);
}

auto createTextConfigForMessage(const core::IMessage &message, const GameView &gameView)
  -> ui::TextConfig
{
  ui::TextConfig config{};

  switch (message.type())
  {
    case core::MessageType::JUMP_REQUESTED:
      return createJumpRequestedMessage(message.as<core::JumpRequestedMessage>());
    case core::MessageType::JUMP_CANCELLED:
      return createJumpCancelledMessage(message.as<core::JumpCancelledMessage>());
    case core::MessageType::LOOT:
      return createLootMessage(message.as<core::LootMessage>(), gameView);
    case core::MessageType::SCANNED:
      return createScannedMessage(message.as<core::ScannedMessage>(), gameView);
    case core::MessageType::SLOT_COMPONENT_UPDATED:
      return createSlotMessage(message.as<core::SlotComponentMessage>());
    default:
      throw std::invalid_argument("Unsupported message type " + str(message.type()));
  }

  return config;
}
} // namespace

void LogUiHandler::subscribeToViews()
{
  auto consumer = [this]() { reset(); };

  auto listener = std::make_unique<IViewListenerProxy>(consumer);
  m_gameView->addListener(std::move(listener));
}

void LogUiHandler::reset()
{
  m_logs.clear();
  m_logsToTrigger.clear();
}

auto LogUiHandler::createMenuFromMessage(const core::IMessage &message) -> ui::UiMenuPtr
{
  const pge::Vec2i pos{m_offset.x - LOG_MENU_DIMS.x / 2, m_offset.y};
  const ui::MenuConfig config{.pos = pos, .dims = LOG_MENU_DIMS, .highlightable = false};
  const ui::BackgroundConfig bg = ui::bgConfigFromColor(pge::colors::TRANSPARENT_WHITE);

  const auto text = createTextConfigForMessage(message, *m_gameView);

  return std::make_unique<ui::UiTextMenu>(config, bg, text);
}

} // namespace bsgalone::client
