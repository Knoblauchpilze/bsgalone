

#include "Server.hh"

namespace bsgalone::server {

Server::Server(net::details::AsioContext &context, const int port, net::IEventBusShPtr eventBus)
  : core::CoreObject("server")
  , m_server(std::make_shared<net::details::AsioServer>(context, port, std::move(eventBus)))
{
  setService("server");
}

void Server::run()
{
  debug("Starting server...");

  m_server->start();

  activeRunLoop();

  debug("Shutting down server...");
}

void Server::requestStop()
{
  m_running.store(false);
  std::unique_lock lock(m_locker);
  m_notifier.notify_one();
}

void Server::activeRunLoop()
{
  m_running.store(true);
  bool running{true};
  while (running)
  {
    std::unique_lock lock(m_locker);
    m_notifier.wait(lock, [this] { return !m_running.load(); });

    running = m_running.load();
  }
}

} // namespace bsgalone::server
