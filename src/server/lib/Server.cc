

#include "Server.hh"
#include <core_utils/TimeUtils.hh>

namespace bsgo {

Server::Server(const int port)
  : utils::CoreObject("server")
  , m_tcpServer(m_context, port)
{
  setService("server");
}

void Server::run()
{
  debug("Starting server...");
  setup();

  activeRunLoop();

  debug("Shutting down server...");
  shutdown();
}

void Server::requestStop()
{
  m_running.store(false);
}

void Server::setup()
{
  info("Starting listening on port " + std::to_string(m_tcpServer.port()));
  m_context.start();
}

void Server::activeRunLoop()
{
  m_running.store(true);
  bool running{true};
  while (running)
  {
    constexpr auto SLEEP_DURATION_WHEN_POLLING_STATUS = utils::Milliseconds(100);
    std::this_thread::sleep_for(SLEEP_DURATION_WHEN_POLLING_STATUS);

    running = m_running.load();
  }
}

void Server::shutdown()
{
  m_context.stop();
}

} // namespace bsgo
