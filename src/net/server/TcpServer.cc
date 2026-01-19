
#include "TcpServer.hh"

#include <iostream>

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

  m_context = std::make_unique<details::AsioContext>();
  m_server  = std::make_shared<details::AsioServer>(*m_context, port, m_eventBus);
  m_server->start();
}

void TcpServer::stop()
{
  const std::lock_guard guard(m_locker);

  auto expected = true;
  if (!m_started.compare_exchange_strong(expected, false))
  {
    error("Unexpected state for server, did you already call stop?");
  }

  std::cout << "[tcp server] shutting down server\n";
  m_server->shutdown();
  std::cout << "[tcp server] reset server\n";
  m_server.reset();
  std::cout << "[tcp server] reset context\n";
  m_context.reset();
  std::cout << "[tcp server] stopped\n";
}

auto TcpServer::trySend(const ClientId clientId, std::vector<char> bytes)
  -> std::optional<MessageId>
{
  return m_server->trySend(clientId, bytes);
}

} // namespace net
