

#include "Server.hh"
#include <core_utils/TimeUtils.hh>

namespace bsgo {
Server::Server()
  : utils::CoreObject("server")
{
  setService("server");
}

void Server::run()
{
  info("Starting server...");
  setup();

  activeRunLoop();

  info("Shutting down server...");
  shutdown();
}

void Server::requestStop()
{
  m_running.store(false);
}

void Server::setup()
{
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
