

#include "GameServer.hh"
#include "TcpServer.hh"

namespace bsgalone::server {

GameServer::GameServer(net::INetworkEventQueueShPtr eventBus)
  : core::CoreObject("server")
  , m_tcpServer(std::make_unique<net::TcpServer>(std::move(eventBus)))
{
  setService("game");
}

void GameServer::run(const int port)
{
  debug("Starting server on port " + std::to_string(port) + "...");

  updateRunningStatus(true);
  startSubSystems(port);

  waitForStopCommand();

  debug("Shutting down server...");

  stopSubSystems();
}

void GameServer::requestStop()
{
  updateRunningStatus(false);
  m_notifier.notify_one();
}

void GameServer::updateRunningStatus(const bool status)
{
  const std::lock_guard guard(m_locker);
  m_running = status;
}

void GameServer::startSubSystems(const int port)
{
  m_tcpServer->start(port);
}

void GameServer::stopSubSystems()
{
  m_tcpServer->stop();
}

void GameServer::waitForStopCommand()
{
  std::unique_lock guard(m_locker);
  m_notifier.wait(guard, [this] { return !m_running; });
}

} // namespace bsgalone::server
