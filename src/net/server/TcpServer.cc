
#include "TcpServer.hh"
#include "ServerStartedEvent.hh"
#include "ServerStoppedEvent.hh"

namespace net {

TcpServer::TcpServer(IEventBusShPtr eventBus)
  : core::CoreObject("tcp")
  , m_eventBus(std::move(eventBus))
{
  setService("net");
  addModule("server");

  if (m_eventBus == nullptr)
  {
    throw std::invalid_argument("Expected non null event bus");
  }
}

void TcpServer::start(const int port)
{
  const std::lock_guard guard(m_locker);

  auto expected = false;
  if (!m_started.compare_exchange_strong(expected, true))
  {
    error("Unexpected state for server, did you already call start?");
  }

  info("Starting listening on port " + std::to_string(port));

  m_context = std::make_unique<details::AsioContext>();
  m_server  = std::make_shared<details::AsioServer>(*m_context, port, m_eventBus);
  m_server->start();

  m_eventBus->pushEvent(std::make_unique<ServerStartedEvent>());
}

void TcpServer::stop()
{
  const std::lock_guard guard(m_locker);

  auto expected = true;
  if (!m_started.compare_exchange_strong(expected, false))
  {
    error("Unexpected state for server, did you already call stop?");
  }

  m_server->shutdown();
  m_server.reset();
  m_context.reset();

  m_eventBus->pushEvent(std::make_unique<ServerStoppedEvent>());
}

auto TcpServer::trySend(const ClientId clientId, std::vector<char> bytes)
  -> std::optional<MessageId>
{
  return m_server->trySend(clientId, bytes);
}

} // namespace net
