
#include "GameSession.hh"

namespace pge {

GameSession::GameSession()
  : core::CoreObject("game")
{
  addModule("session");
}

void GameSession::setupLoadingScreen(const Screen currentScreen, const Screen nextScreen)
{
  if (m_loading)
  {
    error("Unexpected loading screen transition", "Already in loading state");
  }

  m_loading = LoadingData{.previousScreen = currentScreen,
                          .nextScreen     = nextScreen,
                          .transition     = {}};

  debug("Successfully setup loading screen to " + str(nextScreen) + " from " + str(currentScreen));
}

namespace {
bool isTransitionValidForNextScreen(const bsgo::LoadingTransition transition,
                                    const Screen nextScreen)
{
  switch (transition)
  {
    case bsgo::LoadingTransition::JUMP:
      return nextScreen == Screen::GAME;
    case bsgo::LoadingTransition::LOGIN:
      return nextScreen == Screen::OUTPOST;
    case bsgo::LoadingTransition::UNDOCK:
      return nextScreen == Screen::GAME;
    default:
      return false;
  }
}
} // namespace

void GameSession::startLoadingTransition(const bsgo::LoadingTransition transition)
{
  if (!m_loading)
  {
    error("Unexpected start of loading transition", "Not in loading screen");
  }
  if (!isTransitionValidForNextScreen(transition, m_loading->nextScreen))
  {
    error("Unexpected start of loading transition",
          "Transition " + bsgo::str(transition) + " does not match next screen "
            + str(m_loading->nextScreen));
  }

  debug("Starting loading transition to " + str(m_loading->nextScreen));
  m_loading->transition = transition;
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

  debug("Logged out from " + bsgo::str(*m_playerDbId));

  m_playerDbId.reset();
  m_systemDbId.reset();
  m_playerShipDbId.reset();
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
