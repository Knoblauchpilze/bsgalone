
#include "GameRoleUiHandler.hh"
#include "Constants.hh"
#include "GameColorUtils.hh"
#include "IViewListenerProxy.hh"
#include "ScreenCommon.hh"
#include "ShipItemUtils.hh"
#include "StringUtils.hh"

namespace pge {

GameRoleUiHandler::GameRoleUiHandler(const Views &views)
  : IUiHandler("role")
  , m_playerView(views.playerView)
  , m_systemView(views.systemView)
{
  if (nullptr == m_playerView)
  {
    throw std::invalid_argument("Expected non null player view");
  }
  if (nullptr == m_systemView)
  {
    throw std::invalid_argument("Expected non null system view");
  }

  subscribeToViews();
}

void GameRoleUiHandler::initializeMenus(const int width,
                                        const int height,
                                        sprites::TexturePack & /*texturesLoader*/)
{
  const auto viewWidth  = static_cast<int>(MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewHeight = static_cast<int>(MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  Vec2i pos{width - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
            height - viewHeight - VIEW_TO_BOTTOM_OF_SCREEN_IN_PIXELS};
  Vec2i dims{viewWidth, viewHeight};

  m_menu = generateBlankVerticalMenu(pos, dims);
}

bool GameRoleUiHandler::processUserInput(UserInputData &inputData)
{
  return m_menu->processUserInput(inputData);
}

void GameRoleUiHandler::render(Renderer &engine) const
{
  m_menu->render(engine);
}

void GameRoleUiHandler::updateUi()
{
  if (!m_playerView->isReady())
  {
    return;
  }
  if (!m_initialized)
  {
    initializeAvailableShips();
  }
}

void GameRoleUiHandler::subscribeToViews()
{
  auto consumer = [this]() { reset(); };

  auto listener = std::make_unique<IViewListenerProxy>(consumer);
  m_playerView->addListener(std::move(listener));

  listener = std::make_unique<IViewListenerProxy>(consumer);
  m_systemView->addListener(std::move(listener));
}

void GameRoleUiHandler::reset()
{
  m_shipsData.clear();
  m_menu->clearChildren();

  m_initialized = false;
}

void GameRoleUiHandler::initializeAvailableShips()
{
  initializeLayout();
  updateShipMenus();
  m_initialized = true;
}

namespace {
auto findPlayerForShip(const bsgo::Uuid playerDbId, const std::vector<bsgo::PlayerData> &players)
  -> std::optional<bsgo::PlayerData>
{
  const auto maybePlayer = std::find_if(players.begin(),
                                        players.end(),
                                        [playerDbId](const bsgo::PlayerData &playerData) {
                                          return playerData.dbId == playerDbId;
                                        });

  if (maybePlayer == players.end())
  {
    return {};
  }
  return *maybePlayer;
}

auto generateShipDescription(const bsgo::PlayerShipData &ship) -> UiMenuPtr
{
  const MenuConfig config{.highlightable = false};
  auto bg = bgConfigFromColor(colors::BLANK);

  auto desc = std::make_unique<UiMenu>(config, bg);

  desc->addMenu(generateSpacer());

  bg         = bgConfigFromColor(colors::BLANK);
  auto label = bsgo::capitalizeString(ship.name + " (" + bsgo::str(ship.shipClass) + ")");
  auto text  = generateTextConfig(label, colors::GREY, 10);
  auto prop  = std::make_unique<UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  label = bsgo::floatToStr(ship.maxHullPoints, 0) + " hull points (+"
          + bsgo::floatToStr(ship.hullPointsRegen, 2) + "/s)";
  text = generateTextConfig(label);
  prop = std::make_unique<UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  label = bsgo::floatToStr(ship.maxPowerPoints, 0) + " power (+"
          + bsgo::floatToStr(ship.powerRegen, 2) + "/s)";
  text = generateTextConfig(label);
  prop = std::make_unique<UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  desc->addMenu(generateSpacer());

  return desc;
}

auto generatePlayerDescription(const bsgo::PlayerData &player) -> UiMenuPtr
{
  const MenuConfig config{.highlightable = false};
  auto bg = bgConfigFromColor(colors::BLANK);

  auto desc = std::make_unique<UiMenu>(config, bg);

  desc->addMenu(generateSpacer());

  bg              = bgConfigFromColor(colors::BLANK);
  const auto isAi = player.isAi ? std::string("AI") : std::string("Player");
  auto label      = bsgo::capitalizeString(player.name + " (" + isAi + ")");
  auto text       = generateTextConfig(label, colors::GREY, 10);
  auto prop       = std::make_unique<UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  desc->addMenu(generateSpacer());

  return desc;
}

} // namespace

void GameRoleUiHandler::initializeLayout()
{
  const auto faction    = m_playerView->getPlayerFaction();
  const auto palette    = generatePaletteForFaction(faction);
  const auto allPlayers = m_systemView->getSystemPlayers();
  const auto allShips   = m_systemView->getSystemShips();

  const MenuConfig config{.layout = MenuLayout::HORIZONTAL, .highlightable = false};
  const auto bg = bgConfigFromColor(palette.almostOpaqueColor);

  auto shipIndex = 0;
  for (const auto &ship : allShips)
  {
    auto shipMenu          = std::make_unique<UiMenu>(config, bg);
    const auto maybePlayer = findPlayerForShip(ship.playerDbId, allPlayers);
    if (!maybePlayer)
    {
      error("Failed to find player for ship " + bsgo::str(ship.dbId));
    }

    PlayerShipData data{
      .shipDbId   = ship.dbId,
      .playerDbId = maybePlayer->dbId,
      .menu       = shipMenu.get(),
    };
    m_shipsData.emplace_back(std::move(data));

    auto shipDesc   = generateShipDescription(ship);
    auto playerDesc = generatePlayerDescription(*maybePlayer);
    // TODO: The menus are stacked on top of one another
    shipMenu->addMenu(std::move(shipDesc));
    shipMenu->addMenu(std::move(playerDesc));

    auto section = generateInteractiveSection("", [this, shipIndex]() { onShipRequest(shipIndex); });
    m_shipsData.at(shipIndex).button = section.button;

    shipMenu->addMenu(std::move(section.menu));

    m_menu->addMenu(std::move(shipMenu));

    ++shipIndex;
  }
}

namespace {
constexpr auto JOIN_SHIP_BUTTON_TEXT = "Join";
} // namespace

void GameRoleUiHandler::updateShipMenus()
{
  const auto players = m_systemView->getSystemPlayers();
  const auto ships   = m_systemView->getSystemShips();

  auto shipIndex = 0;
  for (auto &shipData : m_shipsData)
  {
    shipData.button->setText(JOIN_SHIP_BUTTON_TEXT);

    /// TODO: Should be based on whether the player already joined
    shipData.button->setEnabled(true);
    shipData.button->setHighlightable(true);
    shipData.state = State::TO_JOIN;
    shipData.button->updateBgColor(colors::DARK_GREEN);

    ++shipIndex;
  }
}

void GameRoleUiHandler::onShipRequest(const int shipIndex)
{
  const auto data = m_shipsData.at(shipIndex);
  switch (data.state)
  {
    case State::TO_JOIN:
      onJoinRequest(shipIndex);
      break;
    default:
      error("Failed to handle ship request",
            "Invalid state " + std::to_string(static_cast<int>(data.state)));
      break;
  }
}

void GameRoleUiHandler::onJoinRequest(const int shipIndex)
{
  if (!m_playerView->isReady())
  {
    return;
  }

  const auto &data = m_shipsData.at(shipIndex);
  // TODO: Should use the ship db id
  m_playerView->tryJoin(data.playerDbId, data.playerDbId);
}

} // namespace pge
