
#include "AbilitiesUiHandler.hh"
#include "MessageListenerWrapper.hh"
#include "MessageUtils.hh"
#include "ScreenCommon.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"

namespace pge {

AbilitiesUiHandler::AbilitiesUiHandler(const bsgo::Views &views)
  : IUiHandler("abilities")
  , AbstractMessageListener({bsgo::MessageType::ENTITY_REMOVED})
  , m_shipView(views.shipView)
  , m_shipDbView(views.shipDbView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
  if (nullptr == m_shipDbView)
  {
    throw std::invalid_argument("Expected non null ship db view");
  }
}

void AbilitiesUiHandler::initializeMenus(const int width, const int height)
{
  generateComputersMenus(width, height);
}

bool AbilitiesUiHandler::processUserInput(UserInputData &inputData)
{
  if (m_disabled)
  {
    return false;
  }

  for (const auto &menu : m_computers)
  {
    if (menu->processUserInput(inputData))
    {
      return true;
    }
  }

  return false;
}

void AbilitiesUiHandler::render(Renderer &engine) const
{
  for (const auto &menu : m_computers)
  {
    menu->render(engine);
  }
}

void AbilitiesUiHandler::updateUi()
{
  if (!m_shipView->isReady())
  {
    return;
  }
  if (!m_initialized)
  {
    initializeAbilities();
  }

  const auto ship = m_shipView->getPlayerShip();

  auto id = 0;
  for (const auto &computer : ship.computers)
  {
    updateComputerMenu(*computer, id);
    ++id;
  }
}

namespace {
constexpr auto NUMBER_OF_ABILITIES = 4;
} // namespace

void AbilitiesUiHandler::reset()
{
  m_ranges.clear();
  m_damages.clear();
  m_statuses.clear();

  for (auto &computer : m_computers)
  {
    computer->clearChildren();
    computer->updateBgColor(semiOpaque(colors::BLACK));
  }

  m_initialized = false;
  m_disabled    = false;
}

void AbilitiesUiHandler::connectToMessageQueue(bsgo::IMessageQueue &messageQueue)
{
  auto listener = std::make_unique<MessageListenerWrapper>(this);
  messageQueue.addListener(std::move(listener));
}

void AbilitiesUiHandler::onMessageReceived(const bsgo::IMessage &message)
{
  if (!m_shipView->isReady())
  {
    return;
  }

  m_disabled = didPlayerShipDied(message.as<bsgo::EntityRemovedMessage>(), *m_shipDbView);
}

void AbilitiesUiHandler::generateComputersMenus(int width, int height)
{
  const Vec2i abilityMenuDims{70, 50};
  constexpr auto SPACING_IN_PIXELS = 5;
  const Vec2i pos{width - NUMBER_OF_ABILITIES * (abilityMenuDims.x + SPACING_IN_PIXELS),
                  height - SPACING_IN_PIXELS - abilityMenuDims.y};

  MenuConfig config{.pos = pos, .dims = abilityMenuDims, .propagateEventsToChildren = false};
  const auto bg = bgConfigFromColor(semiOpaque(colors::BLACK));

  for (auto id = 0u; id < NUMBER_OF_ABILITIES; ++id)
  {
    auto menu = std::make_unique<UiMenu>(config, bg);
    m_computers.push_back(std::move(menu));

    config.pos.x += (abilityMenuDims.x + SPACING_IN_PIXELS);
  }
}

void AbilitiesUiHandler::initializeAbilities()
{
  const auto ship           = m_shipView->getPlayerShip();
  const auto computersCount = ship.computers.size();
  if (computersCount > NUMBER_OF_ABILITIES)
  {
    error("Failed to adapt UI for ship " + ship.str(),
          "Expected maximum " + std::to_string(NUMBER_OF_ABILITIES) + " computer(s), got "
            + std::to_string(ship.computers.size()));
  }

  const MenuConfig config{};
  const auto bg = bgConfigFromColor(colors::BLANK);
  auto textConf = textConfigFromColor("", colors::WHITE);

  for (auto id = 0u; id < computersCount; ++id)
  {
    auto &menu = m_computers[id];

    menu->setClickCallback([this, id]() {
      if (!m_shipView->isReady())
      {
        return;
      }
      m_shipView->tryActivateSlot(id);
    });
    menu->setEnabled(true);

    if (!ship.computers[id]->maybeRange())
    {
      m_ranges.push_back(nullptr);
    }
    else
    {
      auto prop = std::make_unique<UiTextMenu>(config, bg, textConf);
      m_ranges.push_back(prop.get());
      menu->addMenu(std::move(prop));
    }

    if (!ship.computers[id]->damageModifier())
    {
      m_damages.push_back(nullptr);
    }
    else
    {
      auto prop = std::make_unique<UiTextMenu>(config, bg, textConf);
      m_damages.push_back(prop.get());
      menu->addMenu(std::move(prop));
    }

    auto prop = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_statuses.push_back(prop.get());
    menu->addMenu(std::move(prop));
  }

  m_initialized = true;
}

void AbilitiesUiHandler::updateComputerMenu(const bsgo::ComputerSlotComponent &computer,
                                            const int id)
{
  auto &menu = *m_computers[id];

  auto bgColor = semiOpaque(bgColorFromFiringState(computer));
  menu.updateBgColor(bgColor);

  const auto range = computer.maybeRange();
  if (range)
  {
    m_ranges[id]->setText(floatToStr(*range, 0) + "m");
  }

  const auto damage = computer.damageModifier();
  if (damage)
  {
    constexpr auto PERCENTAGE_MULTIPLIER = 100.0f;
    const auto multiplier                = PERCENTAGE_MULTIPLIER * (*damage - 1.0f);
    m_damages[id]->setText("Dmg:" + floatToSignedStr(multiplier, 0) + "%");
  }

  std::string statusText("ready");
  if (!computer.canFire())
  {
    constexpr auto PERCENTAGE_MULTIPLIER = 100.0f;
    statusText = floatToStr(PERCENTAGE_MULTIPLIER * computer.reloadPercentage()) + "%";
  }
  m_statuses[id]->setText(statusText);
}

} // namespace pge
