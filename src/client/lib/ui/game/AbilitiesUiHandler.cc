
#include "AbilitiesUiHandler.hh"
#include "GameColorUtils.hh"
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
  , m_playerView(views.playerView)
{
  if (nullptr == m_shipView)
  {
    throw std::invalid_argument("Expected non null ship view");
  }
  if (nullptr == m_shipDbView)
  {
    throw std::invalid_argument("Expected non null ship db view");
  }
  if (nullptr == m_playerView)
  {
    throw std::invalid_argument("Expected non null player view");
  }
}

void AbilitiesUiHandler::initializeMenus(const int width,
                                         const int height,
                                         sprites::TexturePack &texturesLoader)
{
  constexpr auto SLOTS_TEXTURE_PACK_FILE_PATH = "assets/slots.png";
  const sprites::PackDesc pack{.file   = SLOTS_TEXTURE_PACK_FILE_PATH,
                               .sSize  = Vec2i{600, 600},
                               .layout = Vec2i{2, 1}};
  m_computerTexturesPackId = texturesLoader.registerPack(pack);

  generateComputersMenus(width, height);
}

bool AbilitiesUiHandler::processUserInput(UserInputData &inputData)
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

void AbilitiesUiHandler::render(Renderer &engine) const
{
  for (const auto &menu : m_computers)
  {
    menu->render(engine);
  }
}

void AbilitiesUiHandler::updateUi()
{
  if (!m_shipView->isReady() || !m_playerView->isReady())
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

namespace {
constexpr auto NUMBER_OF_ABILITIES         = 4;
constexpr auto ABILITIES_PICTURE_FILE_PATH = "assets/slot.png";
} // namespace

void AbilitiesUiHandler::generateComputersMenus(int width, int height)
{
  const Vec2i abilityMenuDims{70, 50};
  constexpr auto SPACING_IN_PIXELS = 5;
  const Vec2i pos{width - NUMBER_OF_ABILITIES * (abilityMenuDims.x + SPACING_IN_PIXELS),
                  height - SPACING_IN_PIXELS - abilityMenuDims.y};

  MenuConfig config{.pos                       = pos,
                    .dims                      = abilityMenuDims,
                    .propagateEventsToChildren = false,
                    .customRenderMode          = CustomRenderMode::PRE_RENDER};
  const PictureConfig bg{.path = ABILITIES_PICTURE_FILE_PATH};

  for (auto id = 0u; id < NUMBER_OF_ABILITIES; ++id)
  {
    auto menu = std::make_unique<UiPictureMenu>(config, bg);
    m_computers.push_back(std::move(menu));

    config.pos.x += (abilityMenuDims.x + SPACING_IN_PIXELS);
  }
}

namespace {
auto tryGetDbComputer(const bsgo::Uuid computerDbId,
                      const std::vector<bsgo::PlayerComputer> &computers)
  -> std::optional<bsgo::PlayerComputer>
{
  for (const auto &computer : computers)
  {
    if (computer.id == computerDbId)
    {
      return computer;
    }
  }

  return {};
}

auto spriteIdFromComputer(const bsgo::PlayerComputer &computer) -> Vec2i
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
  const auto ship           = m_shipView->getPlayerShip();
  const auto dbComputers    = m_shipDbView->getPlayerShipComputers();
  const auto computersCount = ship.computers.size();
  if (computersCount > NUMBER_OF_ABILITIES)
  {
    error("Failed to adapt UI for ship " + ship.str(),
          "Expected maximum " + std::to_string(NUMBER_OF_ABILITIES) + " computer(s), got "
            + std::to_string(ship.computers.size()));
  }

  const auto palette = generatePaletteForFaction(m_playerView->getPlayerFaction());

  const MenuConfig config{};
  const auto bg = bgConfigFromColor(colors::BLANK);
  auto textConf = textConfigFromColor("", palette.defaultColor);

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
      error("Failed to initialize computers for ship " + bsgo::str(ship.dbComp().dbId()),
            "Failed to find component for computer " + bsgo::str(computerDbId));
    }

    menu->setClickCallback([this, id]() {
      if (!m_shipView->isReady())
      {
        return;
      }
      m_shipView->tryActivateSlot(id);
    });
    menu->setEnabled(true);

    debug("sprite for " + dbComputer->name + " is " + spriteIdFromComputer(*dbComputer).str());

    menu->setSprite(m_computerTexturesPackId, spriteIdFromComputer(*dbComputer));

    auto prop = std::make_unique<UiTextMenu>(config, bg, textConf);
    m_statuses.push_back(prop.get());
    menu->addMenu(std::move(prop));

    ++id;
  }

  m_initialized = true;
}

void AbilitiesUiHandler::updateComputerMenu(const bsgo::ComputerSlotComponent &computer,
                                            const int id)
{
  auto &menu = *m_computers[id];

  auto bgColor = semiOpaque(bgColorFromFiringState(computer));
  menu.updateBgColor(bgColor);

  std::string statusText("ready");
  if (!computer.canFire())
  {
    constexpr auto PERCENTAGE_MULTIPLIER = 100.0f;
    statusText = bsgo::floatToStr(PERCENTAGE_MULTIPLIER * computer.reloadPercentage()) + "%";
  }
  m_statuses[id]->setText(statusText);
}

} // namespace pge
