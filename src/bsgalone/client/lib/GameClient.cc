

#include "GameClient.hh"
#include "TcpClient.hh"

namespace bsgalone::client {
constexpr auto DEFAULT_SERVER_URL = "127.0.0.1";

GameClient::GameClient(net::IEventBusShPtr eventBus)
  : core::CoreObject("client")
  , m_tcpClient(std::make_unique<net::TcpClient>(std::move(eventBus)))
{
  setService("game");
}

void GameClient::run(const int port)
{
  const auto url = std::format("{}:{}", DEFAULT_SERVER_URL, port);
  debug("Connecting to server at " + url + "...");

  updateRunningStatus(true);
  startSubSystems(port);

  waitForStopCommand();

  debug("Shutting down client...");

  stopSubSystems();
}

void GameClient::requestStop()
{
  updateRunningStatus(false);
  m_notifier.notify_one();
}

void GameClient::updateRunningStatus(const bool status)
{
  const std::lock_guard guard(m_locker);
  m_running = status;
}

void GameClient::startSubSystems(const int port)
{
  m_tcpClient->connect(DEFAULT_SERVER_URL, port);
}

void GameClient::stopSubSystems()
{
  m_tcpClient->disconnect();
}

void GameClient::waitForStopCommand()
{
  std::unique_lock guard(m_locker);
  m_notifier.wait(guard, [this] { return !m_running; });
}

} // namespace bsgalone::client
