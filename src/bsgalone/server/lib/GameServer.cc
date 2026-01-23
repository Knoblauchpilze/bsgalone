

#include "GameServer.hh"

namespace bsgalone::server {

GameServer::GameServer()
  : core::CoreObject("server")
{
  setService("game");
}

void GameServer::run(const int port)
{
  debug("Starting server on port " + std::to_string(port) + "...");

  {
    const std::lock_guard guard(m_locker);
    m_running = true;
  }

  std::unique_lock guard(m_locker);
  m_notifier.wait(guard, [this] { return !m_running; });

  debug("Shutting down server...");
}

void GameServer::requestStop()
{
  {
    const std::lock_guard guard(m_locker);
    m_running = false;
  }
  m_notifier.notify_one();
}

} // namespace bsgalone::server
