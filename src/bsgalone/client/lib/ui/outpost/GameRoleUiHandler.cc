
#include "GameRoleUiHandler.hh"
#include "Constants.hh"
#include "GameColorUtils.hh"
#include "IViewListenerProxy.hh"
#include "ScreenCommon.hh"
#include "ShipItemUtils.hh"
#include "StringUtils.hh"

namespace bsgalone::client {

GameRoleUiHandler::GameRoleUiHandler(const Views &views)
  : AbstractUiHandler("role")
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
                                        pge::sprites::TexturePack & /*texturesLoader*/)
{
  const auto viewWidth  = static_cast<int>(MAIN_VIEW_WIDTH_TO_SCREEN_WIDTH_RATIO * width);
  const auto viewHeight = static_cast<int>(MAIN_VIEW_HEIGHT_TO_SCREEN_HEIGHT_RATIO * height);

  pge::Vec2i pos{width - viewWidth - VIEW_TO_RIGHT_OF_SCREEN_IN_PIXELS,
                 height - viewHeight - VIEW_TO_BOTTOM_OF_SCREEN_IN_PIXELS};
  pge::Vec2i dims{viewWidth, viewHeight};

  m_menu = generateBlankVerticalMenu(pos, dims);
}

bool GameRoleUiHandler::processUserInput(ui::UserInputData &inputData)
{
  return m_menu->processUserInput(inputData);
}

void GameRoleUiHandler::render(pge::Renderer &engine) const
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
auto findPlayerForShip(const core::Uuid playerDbId, const std::vector<core::PlayerData> &players)
  -> std::optional<core::PlayerData>
{
  const auto maybePlayer = std::find_if(players.begin(),
                                        players.end(),
                                        [playerDbId](const core::PlayerData &playerData) {
                                          return playerData.dbId == playerDbId;
                                        });

  if (maybePlayer == players.end())
  {
    return {};
  }
  return *maybePlayer;
}

auto generateShipDescription(const core::PlayerShipData &ship, const core::PlayerData &player)
  -> ui::UiMenuPtr
{
  const ui::MenuConfig config{.highlightable = false};
  auto bg = ui::bgConfigFromColor(pge::colors::BLANK);

  auto desc = std::make_unique<ui::UiMenu>(config, bg);

  desc->addMenu(generateSpacer());

  bg              = ui::bgConfigFromColor(pge::colors::BLANK);
  const auto isAi = player.isAi ? std::string("AI") : std::string("Player");
  auto label      = core::capitalizeString(player.name + " (" + isAi + ")");
  auto text       = generateTextConfig(label, pge::colors::GREY, 10);
  auto prop       = std::make_unique<ui::UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  bg    = ui::bgConfigFromColor(pge::colors::BLANK);
  label = core::capitalizeString(ship.name + " (" + core::str(ship.shipClass) + ")");
  text  = generateTextConfig(label, pge::colors::GREY, 10);
  prop  = std::make_unique<ui::UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  label = core::floatToStr(ship.maxHullPoints, 0) + " hull points (+"
          + core::floatToStr(ship.hullPointsRegen, 2) + "/s)";
  text = generateTextConfig(label);
  prop = std::make_unique<ui::UiTextMenu>(config, bg, text);
  desc->addMenu(std::move(prop));

  label = core::floatToStr(ship.maxPowerPoints, 0) + " power (+"
          + core::floatToStr(ship.powerRegen, 2) + "/s)";
  text = generateTextConfig(label);
  prop = std::make_unique<ui::UiTextMenu>(config, bg, text);
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

  const ui::MenuConfig config{.layout = ui::MenuLayout::HORIZONTAL, .highlightable = false};
  const auto bg = ui::bgConfigFromColor(palette.almostOpaqueColor);

  auto shipIndex = 0;
  for (const auto &ship : allShips)
  {
    auto shipMenu          = std::make_unique<ui::UiMenu>(config, bg);
    const auto maybePlayer = findPlayerForShip(ship.playerDbId, allPlayers);
    if (!maybePlayer)
    {
      error("Failed to find player for ship " + core::str(ship.dbId));
    }

    PlayerShipData data{
      .shipDbId   = ship.dbId,
      .playerDbId = maybePlayer->dbId,
      .menu       = shipMenu.get(),
    };
    m_shipsData.emplace_back(std::move(data));

    auto shipDesc = generateShipDescription(ship, *maybePlayer);
    shipMenu->addMenu(std::move(shipDesc));

    auto section = generateInteractiveSection("", [this, shipIndex]() { onShipRequest(shipIndex); });
    m_shipsData.at(shipIndex).button = section.button;

    shipMenu->addMenu(std::move(section.menu));

    m_menu->addMenu(std::move(shipMenu));

    ++shipIndex;
  }
}

namespace {
constexpr auto JOIN_SHIP_BUTTON_TEXT        = "Join";
constexpr auto JOINED_SHIP_BUTTON_TEXT      = "Joined";
constexpr auto UNAVAILABLE_SHIP_BUTTON_TEXT = "Unavailable";

/// @brief - Tries to find the player attached to a ship. This function will traverse
/// the list of ship and try to find one that is attached to the provided ship.
/// @param shipDbId - the identifier of the ship to find
/// @param players - the list of players potentially attached to the ship
/// @return - an optional value representing (when filled) the player attached to the
/// ship.
auto tryFindPlayerForShip(const core::Uuid shipDbId, const std::vector<core::PlayerData> &players)
  -> std::optional<core::PlayerData>
{
  const auto maybePlayer = std::find_if(players.begin(),
                                        players.end(),
                                        [shipDbId](const core::PlayerData &playerData) {
                                          return playerData.attachedShip
                                                 && *playerData.attachedShip == shipDbId;
                                        });

  if (maybePlayer == players.end())
  {
    return {};
  }

  return *maybePlayer;
}

/// @brief - Used to determine the text to show on the join ship button.
/// @param joined - whether or not somebody joined this ship
/// @param playerJoined - whether or not the player currently logged in joined the ship
/// @return - the text to display on the join ship button.
auto determineButtonText(const bool joined, const bool playerJoined) -> std::string
{
  if (!joined)
  {
    return JOIN_SHIP_BUTTON_TEXT;
  }
  if (playerJoined)
  {
    return JOINED_SHIP_BUTTON_TEXT;
  }
  return UNAVAILABLE_SHIP_BUTTON_TEXT;
}
} // namespace

void GameRoleUiHandler::updateShipMenus()
{
  const auto playerDbId = m_playerView->gameSession().getPlayerDbId();
  const auto player     = m_systemView->getPlayer(playerDbId);

  const auto players = m_systemView->getSystemPlayers();
  const auto ships   = m_systemView->getSystemShips();

  auto shipIndex = 0;
  for (auto &shipData : m_shipsData)
  {
    const auto maybeOwner = tryFindPlayerForShip(shipData.shipDbId, players);

    const auto joined       = maybeOwner.has_value();
    const auto playerJoined = maybeOwner && maybeOwner->dbId == player.dbId;

    shipData.button->setEnabled(!joined);
    shipData.button->setHighlightable(!joined);
    shipData.state   = joined ? State::JOINED : State::TO_JOIN;
    const auto color = joined ? pge::colors::DARK_GREY : pge::colors::DARK_GREEN;
    const auto text  = determineButtonText(joined, playerJoined);
    shipData.button->setText(text);
    shipData.button->updateBgColor(color);

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
  m_playerView->tryJoin(m_playerView->gameSession().getPlayerDbId(), data.shipDbId);
}

} // namespace bsgalone::client
