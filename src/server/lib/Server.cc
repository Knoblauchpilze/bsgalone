

#include "Server.hh"

namespace bsgo {
Server::Server()
  : utils::CoreObject("server")
{}

void Server::start()
{
  m_running.store(true);

  info("Starting server");

  bool running{true};
  while (running)
  {
    running = m_running.load();
  }

  info("Shutdown server");
}

void Server::stop()
{
  m_running.store(false);
}

} // namespace bsgo
