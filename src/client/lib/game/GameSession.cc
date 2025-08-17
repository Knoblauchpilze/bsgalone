
#include "GameSession.hh"

namespace pge {

GameSession::GameSession()
  : core::CoreObject("game")
{
  addModule("session");
}

namespace {
auto determineNextScreen(const bsgo::LoadingTransition transition)
{
  switch (transition)
  {
    case bsgo::LoadingTransition::ACTIVE_SHIP_CHANGED:
      return Screen::OUTPOST;
    case bsgo::LoadingTransition::DOCK:
      return Screen::OUTPOST;
    case bsgo::LoadingTransition::EQUIP:
      return Screen::OUTPOST;
    case bsgo::LoadingTransition::JUMP:
      return Screen::GAME;
    case bsgo::LoadingTransition::LOGIN:
      return Screen::OUTPOST;
    case bsgo::LoadingTransition::PURCHASE:
      return Screen::OUTPOST;
    case bsgo::LoadingTransition::UNDOCK:
      return Screen::GAME;
    default:
      throw std::invalid_argument("Unsupported transition " + bsgo::str(transition));
  }
}

bool isTransitionValidForCurrentScreen(const Screen currentScreen,
                                       const bsgo::LoadingTransition transition)
{
  switch (transition)
  {
    case bsgo::LoadingTransition::ACTIVE_SHIP_CHANGED:
      return currentScreen == Screen::OUTPOST;
    case bsgo::LoadingTransition::DOCK:
      return currentScreen == Screen::GAME;
    case bsgo::LoadingTransition::EQUIP:
      return currentScreen == Screen::OUTPOST;
    case bsgo::LoadingTransition::JUMP:
      return currentScreen == Screen::GAME;
    case bsgo::LoadingTransition::LOGIN:
      return currentScreen == Screen::LOGIN;
    case bsgo::LoadingTransition::PURCHASE:
      return currentScreen == Screen::OUTPOST;
    case bsgo::LoadingTransition::UNDOCK:
      return currentScreen == Screen::OUTPOST;
    default:
      return false;
  }
}
} // namespace

void GameSession::startLoadingTransition(const Screen currentScreen,
                                         const bsgo::LoadingTransition transition)
{
  if (m_loading)
  {
    error("Unexpected start of loading transition " + bsgo::str(transition),
          "Already in loading screen");
  }

  const auto nextScreen = determineNextScreen(transition);

  if (!isTransitionValidForCurrentScreen(currentScreen, transition))
  {
    error("Unexpected start of loading transition " + bsgo::str(transition),
          "Transition " + bsgo::str(transition) + " does not match next screen "
            + str(m_loading->nextScreen));
  }

  m_loading = LoadingData{.previousScreen = currentScreen,
                          .nextScreen     = nextScreen,
                          .transition     = transition};

  debug("Starting loading transition to " + str(m_loading->nextScreen));
}

auto GameSession::finishLoadingTransition(const bsgo::LoadingTransition transition)
  -> ScreenTransition
{
  if (!m_loading)
  {
    error("Unexpected end of loading transition", "Not in loading screen");
  }
  if (!m_loading->transition || m_loading->transition != transition)
  {
    error("Unexpected end of loading transition", "Transition does not match");
  }

  const auto previousScreen = m_loading->previousScreen;
  const auto nextScreen     = m_loading->nextScreen;

  debug("Ending loading transition from " + str(previousScreen) + " to " + str(nextScreen));

  m_loading.reset();

  return {.previous = previousScreen, .next = nextScreen};
}

void GameSession::onPlayerLoggedIn(const bsgo::Uuid playerDbId)
{
  if (m_playerDbId)
  {
    error("Unexpected player login", "Already logged in as " + bsgo::str(*m_playerDbId));
  }

  m_playerDbId = playerDbId;

  debug("Logged in as " + bsgo::str(playerDbId));
}

void GameSession::onPlayerLoggedOut()
{
  if (!m_playerDbId)
  {
    error("Unexpected player logout", "No player logged in");
  }
  if (!m_faction)
  {
    error("Unexpected player logout", "No faction defined for player");
  }

  debug("Logged out from " + bsgo::str(*m_playerDbId));

  m_playerDbId.reset();
  m_faction.reset();
  m_systemDbId.reset();
  m_playerShipDbId.reset();
}

void GameSession::setPlayerFaction(const bsgo::Faction faction)
{
  if (!m_playerDbId)
  {
    error("Unable to set faction", "No player logged in");
  }

  m_faction = faction;
}

bool GameSession::hasPlayerDbId() const
{
  return m_playerDbId.has_value();
}

auto GameSession::getPlayerDbId() const -> bsgo::Uuid
{
  if (!m_playerDbId)
  {
    error("Failed to get player DB ID", "No player logged in");
  }

  return *m_playerDbId;
}

auto GameSession::getFaction() const -> bsgo::Faction
{
  if (!m_faction)
  {
    error("Failed to get faction", "No faction defined for player");
  }

  return *m_faction;
}

bool GameSession::hasSystemDbId() const
{
  return m_systemDbId.has_value();
}

auto GameSession::getSystemDbId() const -> bsgo::Uuid
{
  if (!m_systemDbId)
  {
    error("Failed to get system DB ID", "No system defined");
  }

  return *m_systemDbId;
}

bool GameSession::hasPlayerActiveShipDbId() const
{
  return m_playerShipDbId.has_value();
}

auto GameSession::getPlayerActiveShipDbId() const -> bsgo::Uuid
{
  if (!m_playerShipDbId)
  {
    error("Failed to get player ship DB ID", "No active ship");
  }

  return *m_playerShipDbId;
}

void GameSession::onActiveSystemChanged(const bsgo::Uuid systemDbId)
{
  m_systemDbId = systemDbId;
  debug("Active system changed to " + bsgo::str(*m_systemDbId));
}

void GameSession::onActiveShipChanged(const bsgo::Uuid shipDbId)
{
  m_playerShipDbId = shipDbId;
  debug("Active ship changed to " + bsgo::str(*m_playerShipDbId));
}

} // namespace pge
