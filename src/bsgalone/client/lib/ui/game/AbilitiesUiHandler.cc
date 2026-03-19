
#include "AbilitiesUiHandler.hh"
#include "GameColorUtils.hh"
#include "IViewListenerProxy.hh"
#include "MessageListenerWrapper.hh"
#include "MessageUtils.hh"
#include "ScreenCommon.hh"
#include "SlotComponentUtils.hh"
#include "StringUtils.hh"

namespace bsgalone::client {

AbilitiesUiHandler::AbilitiesUiHandler(const Views &views)
  : AbstractUiHandler("abilities")
  , AbstractMessageListener({core::MessageType::ENTITY_REMOVED})
  , m_gameView(views.gameView)
{
  if (nullptr == m_gameView)
  {
    throw std::invalid_argument("Expected non null game view");
  }

  subscribeToViews();
}

void AbilitiesUiHandler::initializeMenus(const int width,
                                         const int height,
                                         pge::sprites::TexturePack &texturesLoader)
{
  constexpr auto SLOTS_TEXTURE_PACK_FILE_PATH = "assets/slots.png";
  const pge::sprites::PackDesc pack{.file   = SLOTS_TEXTURE_PACK_FILE_PATH,
                                    .sSize  = pge::Vec2i{600, 600},
                                    .layout = pge::Vec2i{2, 1}};
  m_computerTexturesPackId = texturesLoader.registerPack(pack);

  generateComputersMenus(width, height);
}

bool AbilitiesUiHandler::processUserInput(ui::UserInputData &inputData)
{
  if (m_disabled)
  {
    return false;
  }

  auto out{false};
  for (const auto &menu : m_computers)
  {
    out |= menu->processUserInput(inputData);
  }

  return out;
}

void AbilitiesUiHandler::render(pge::Renderer &engine) const
{
  for (const auto &menu : m_computers)
  {
    menu->render(engine);
  }
}

void AbilitiesUiHandler::updateUi()
{
  if (!m_gameView->isReady())
  {
    return;
  }
  if (!m_initialized)
  {
    initializeAbilities();
  }

  const auto ship = m_gameView->getPlayerShip();

  auto id = 0;
  for (const auto &computer : ship.computers)
  {
    updateComputerMenu(*computer, id);
    ++id;
  }
}

void AbilitiesUiHandler::connectToMessageQueue(core::IMessageQueue &messageQueue)
{
  auto listener = std::make_unique<MessageListenerWrapper>(this);
  messageQueue.addListener(std::move(listener));
}

void AbilitiesUiHandler::onEventReceived(const core::IMessage &message)
{
  if (!m_gameView->isReady())
  {
    return;
  }

  m_disabled = didPlayerShipDie(message.as<core::EntityRemovedMessage>(), *m_gameView);
}

void AbilitiesUiHandler::subscribeToViews()
{
  auto consumer = [this]() { reset(); };

  auto listener = std::make_unique<IViewListenerProxy>(consumer);
  m_gameView->addListener(std::move(listener));
}

void AbilitiesUiHandler::reset()
{
  m_statuses.clear();

  for (auto &computer : m_computers)
  {
    computer->clearChildren();
  }

  m_initialized = false;
  m_disabled    = false;
}

namespace {
constexpr auto NUMBER_OF_ABILITIES         = 4;
constexpr auto ABILITIES_PICTURE_FILE_PATH = "assets/slot.png";
} // namespace

void AbilitiesUiHandler::generateComputersMenus(int width, int height)
{
  const pge::Vec2i abilityMenuDims{70, 50};
  constexpr auto SPACING_IN_PIXELS = 5;
  const pge::Vec2i pos{width - NUMBER_OF_ABILITIES * (abilityMenuDims.x + SPACING_IN_PIXELS),
                       height - SPACING_IN_PIXELS - abilityMenuDims.y};

  ui::MenuConfig config{.pos                       = pos,
                        .dims                      = abilityMenuDims,
                        .propagateEventsToChildren = false,
                        .customRenderMode          = ui::CustomRenderMode::PRE_RENDER};
  const ui::PictureConfig bg{.path = ABILITIES_PICTURE_FILE_PATH};

  for (auto id = 0u; id < NUMBER_OF_ABILITIES; ++id)
  {
    auto menu = std::make_unique<ui::UiPictureMenu>(config, bg);
    m_computers.push_back(std::move(menu));

    config.pos.x += (abilityMenuDims.x + SPACING_IN_PIXELS);
  }
}

namespace {
auto tryGetDbComputer(const core::Uuid computerDbId,
                      const std::vector<core::PlayerComputerData> &computers)
  -> std::optional<core::PlayerComputerData>
{
  for (const auto &computer : computers)
  {
    if (computer.dbId == computerDbId)
    {
      return computer;
    }
  }

  return {};
}

auto spriteIdFromComputer(const core::PlayerComputerData &computer) -> pge::Vec2i
{
  if (computer.name == "Scan")
  {
    return {0, 0};
  }

  if (computer.name == "Weapon buff")
  {
    return {1, 0};
  }

  throw std::invalid_argument("Unsupported computer " + computer.name);
}

} // namespace

void AbilitiesUiHandler::initializeAbilities()
{
  const auto ship           = m_gameView->getPlayerShip();
  const auto dbComputers    = m_gameView->getPlayerShipComputers();
  const auto computersCount = ship.computers.size();
  if (computersCount > NUMBER_OF_ABILITIES)
  {
    error("Failed to adapt UI for ship " + ship.str(),
          "Expected maximum " + std::to_string(NUMBER_OF_ABILITIES) + " computer(s), got "
            + std::to_string(ship.computers.size()));
  }

  const auto palette = generatePaletteForFaction(m_gameView->getPlayerFaction());

  const ui::MenuConfig config{};
  const auto bg = ui::bgConfigFromColor(pge::colors::BLANK);
  auto textConf = ui::textConfigFromColor("", palette.defaultColor);

  for (const auto &menu : m_computers)
  {
    resetPictureMenuToDefault(*menu, palette.defaultColor);
  }

  auto id = 0;
  for (const auto &computer : ship.computers)
  {
    auto &menu              = m_computers[id];
    const auto computerDbId = computer->dbId();
    const auto dbComputer   = tryGetDbComputer(computerDbId, dbComputers);
    if (!dbComputer)
    {
      error("Failed to initialize computers for ship " + core::str(ship.dbComp().dbId()),
            "Failed to find component for computer " + core::str(computerDbId));
    }

    menu->setClickCallback([this, id]() {
      if (!m_gameView->isReady())
      {
        return;
      }
      m_gameView->tryActivateSlot(id);
    });
    menu->setEnabled(true);

    debug("sprite for " + dbComputer->name + " is " + spriteIdFromComputer(*dbComputer).str());

    menu->setSprite(m_computerTexturesPackId, spriteIdFromComputer(*dbComputer));

    auto prop = std::make_unique<ui::UiTextMenu>(config, bg, textConf);
    m_statuses.push_back(prop.get());
    menu->addMenu(std::move(prop));

    ++id;
  }

  m_initialized = true;
}

void AbilitiesUiHandler::updateComputerMenu(const core::ComputerSlotComponent &computer,
                                            const int id)
{
  auto &menu = *m_computers[id];

  auto bgColor = semiOpaque(bgColorFromFiringState(computer));
  menu.updateBgColor(bgColor);

  std::string statusText("ready");
  if (!computer.canFire())
  {
    constexpr auto PERCENTAGE_MULTIPLIER = 100.0f;
    statusText = core::floatToStr(PERCENTAGE_MULTIPLIER * computer.reloadPercentage()) + "%";
  }
  m_statuses[id]->setText(statusText);
}

} // namespace bsgalone::client
