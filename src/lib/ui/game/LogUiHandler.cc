
#include "LogUiHandler.hh"
#include "GameColorUtils.hh"
#include "StringUtils.hh"
#include "UiTextMenu.hh"

#include "JumpMessage.hh"
#include "LootMessage.hh"
#include "ScannedMessage.hh"
#include "SlotMessage.hh"

namespace pge {

const std::unordered_set<bsgo::MessageType> RELEVANT_MESSAGE_TYPES_TO_LOG
  = {bsgo::MessageType::JUMP,
     bsgo::MessageType::LOOT,
     bsgo::MessageType::SCANNED,
     bsgo::MessageType::SLOT};

LogUiHandler::LogUiHandler(const bsgo::Views &views)
  : IUiHandler("log")
  , AbstractMessageListener(RELEVANT_MESSAGE_TYPES_TO_LOG)
  , m_systemView(views.systemView)
  , m_resourceView(views.resourceView)
  , m_shipView(views.shipView)
{
  if (nullptr == m_systemView)
  {
    throw std::invalid_argument("Expected non null system view");
  }
  if (nullptr == m_resourceView)
  {
    throw std::invalid_argument("Expected non null resource view");
  }
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
}

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
  for (const auto &message : m_logs)
  {
    message.menu->render(engine.getRenderer());
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

void LogUiHandler::reset()
{
  m_logs.clear();
  m_logsToTrigger.clear();
}

void LogUiHandler::connectToMessageQueue(bsgo::IMessageQueue &messageQueue)
{
  messageQueue.addListener(this);
}

namespace {
constexpr std::size_t MAXIMUM_NUMBER_OF_LOGS_DISPLAYED = 5;
const auto LOG_MENU_DIMS                               = olc::vi2d{150, 20};
constexpr auto LOG_FADE_OUT_DURATION_MS                = 7000;

const std::unordered_set<bsgo::JumpState> JUMP_STATES_NOT_TRIGGERING_LOG
  = {bsgo::JumpState::RUNNING, bsgo::JumpState::COMPLETED};

bool shouldMessageBeFiltered(const bsgo::IMessage &message, const bsgo::Entity &playerShip)
{
  if (bsgo::MessageType::JUMP == message.type())
  {
    const auto &jumpMessage = message.as<bsgo::JumpMessage>();
    return JUMP_STATES_NOT_TRIGGERING_LOG.contains(jumpMessage.getJumpState());
  }

  if (bsgo::MessageType::SLOT == message.type())
  {
    const auto &slotMessage = message.as<bsgo::SlotMessage>();
    if (bsgo::Slot::WEAPON == slotMessage.getSlotType())
    {
      return true;
    }

    const auto computerIsOffensive = playerShip.computers.at(slotMessage.getSlotIndex())
                                       ->isOffensive();
    const auto computerIsFired = bsgo::SlotState::FIRED == slotMessage.getSlotState();
    return computerIsOffensive || !computerIsFired;
  }

  return false;
}
} // namespace

void LogUiHandler::onMessageReceived(const bsgo::IMessage &message)
{
  if (!m_shipView->isReady())
  {
    return;
  }

  if (m_logs.size() > MAXIMUM_NUMBER_OF_LOGS_DISPLAYED)
  {
    m_logs.pop_back();
  }

  const auto ship = m_shipView->getPlayerShip();
  if (shouldMessageBeFiltered(message, ship))
  {
    return;
  }

  auto logMenu = createMenuFromMessage(message);
  LogMessage data{};
  data.rawMenu = logMenu.get();

  TimedMenuConfig timedConfig{.duration          = utils::Milliseconds(LOG_FADE_OUT_DURATION_MS),
                              .fadeOut           = true,
                              .fadeOutDuration   = utils::Milliseconds(LOG_FADE_OUT_DURATION_MS),
                              .applyToBackGround = false};
  data.menu = std::make_unique<UiTimedMenu>(timedConfig, std::move(logMenu));

  auto id = 1;
  for (const auto &data : m_logs)
  {
    const auto y = m_offset.y + id * LOG_MENU_DIMS.y;
    const olc::vi2d pos{m_offset.x - LOG_MENU_DIMS.x / 2, y};
    data.rawMenu->setPosition(pos);
    ++id;
  }

  m_logsToTrigger.push_back(data.menu.get());
  m_logs.emplace_front(std::move(data));
}

namespace {
constexpr auto FTL_JUMP_STARTED_TEXT   = "FTL jump sequence started";
constexpr auto FTL_JUMP_CANCELLED_TEXT = "FTL jump sequence aborted";

auto createJumpMessage(const bsgo::JumpMessage &message)
{
  const auto color = olc::WHITE;

  switch (message.getJumpState())
  {
    case bsgo::JumpState::STARTED:
      return textConfigFromColor(FTL_JUMP_STARTED_TEXT, color);
    case bsgo::JumpState::CANCELLED:
      return textConfigFromColor(FTL_JUMP_CANCELLED_TEXT, color);
    default:
      throw std::invalid_argument("Unsupported jump state to produce message");
  }
}

auto createLootMessage(const bsgo::LootMessage &message, const bsgo::ResourceView &resourceView)
  -> TextConfig
{
  const auto resource = resourceView.getResourceName(message.resourceId());
  const auto text     = "+" + floatToStr(message.amount(), 0) + " " + resource;
  const auto color    = colorFromResourceName(resource);
  return textConfigFromColor(text, color);
}

constexpr auto NO_USEFUL_RESOURCES_TEXT = "Mineral analysis: no useful resources";

auto createScannedMessage(const bsgo::ScannedMessage &message,
                          const bsgo::SystemView &systemView,
                          const bsgo::ResourceView &resourceView) -> TextConfig
{
  const auto asteroid = systemView.getAsteroid(message.asteroidEntityId());

  if (!asteroid.exists<bsgo::LootComponent>())
  {
    return textConfigFromColor(NO_USEFUL_RESOURCES_TEXT, olc::WHITE);
  }

  if (asteroid.resources.size() > 1)
  {
    throw std::invalid_argument("Expected asteroid to contain only one resource as loot but got "
                                + std::to_string(asteroid.resources.size()));
  }
  const auto loot = asteroid.resources.at(0);

  const auto resource = resourceView.getResourceName(loot->resource());
  const auto text     = "Mineral analysis: " + floatToStr(loot->amount(), 0) + " " + resource;
  const auto color    = colorFromResourceName(resource);
  return textConfigFromColor(text, color);
}

constexpr auto ELECTRONIC_SUPPORT_TEXT = "Your systems are being improved by electronic support";

auto createSlotMessage(const bsgo::SlotMessage & /*message*/) -> TextConfig
{
  return textConfigFromColor(ELECTRONIC_SUPPORT_TEXT, olc::APPLE_GREEN);
}

auto createTextConfigForMessage(const bsgo::IMessage &message,
                                const bsgo::SystemView &systemView,
                                const bsgo::ResourceView &resourceView) -> TextConfig
{
  TextConfig config{};

  switch (message.type())
  {
    case bsgo::MessageType::JUMP:
      return createJumpMessage(message.as<bsgo::JumpMessage>());
    case bsgo::MessageType::LOOT:
      return createLootMessage(message.as<bsgo::LootMessage>(), resourceView);
    case bsgo::MessageType::SCANNED:
      return createScannedMessage(message.as<bsgo::ScannedMessage>(), systemView, resourceView);
    case bsgo::MessageType::SLOT:
      return createSlotMessage(message.as<bsgo::SlotMessage>());
    default:
      throw std::invalid_argument("Unsupported message type " + bsgo::str(message.type()));
  }

  return config;
}
} // namespace

auto LogUiHandler::createMenuFromMessage(const bsgo::IMessage &message) -> UiMenuPtr
{
  const olc::vi2d pos{m_offset.x - LOG_MENU_DIMS.x / 2, m_offset.y};
  const MenuConfig config{.pos = pos, .dims = LOG_MENU_DIMS, .highlightable = false};
  const BackgroundConfig bg = bgConfigFromColor(transparent(olc::WHITE, alpha::Transparent));

  const auto text = createTextConfigForMessage(message, *m_systemView, *m_resourceView);

  return std::make_unique<UiTextMenu>(config, bg, text);
}

} // namespace pge
