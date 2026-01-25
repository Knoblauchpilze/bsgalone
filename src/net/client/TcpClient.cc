
#include "TcpClient.hh"
#include "ServerStartedEvent.hh"
#include "ServerStoppedEvent.hh"

namespace net {

TcpClient::TcpClient(IEventBusShPtr eventBus)
  : core::CoreObject("tcp")
  , m_eventBus(std::move(eventBus))
{
  setService("net");
  addModule("client");

  if (m_eventBus == nullptr)
  {
    throw std::invalid_argument("Expected non null event bus");
  }
}

void TcpClient::connect(const std::string &url, const int port)
{
  const std::lock_guard guard(m_locker);

  auto expected = false;
  if (!m_started.compare_exchange_strong(expected, true))
  {
    error("Unexpected state for client, did you already call connect?");
  }

  m_context = std::make_unique<details::AsioContext>();
  m_client  = std::make_shared<details::AsioClient>(m_eventBus);
  m_client->connect(*m_context, url, port);

  m_eventBus->pushEvent(std::make_unique<ServerStartedEvent>());
}

void TcpClient::disconnect()
{
  const std::lock_guard guard(m_locker);

  auto expected = true;
  if (!m_started.compare_exchange_strong(expected, false))
  {
    error("Unexpected state for client, did you already call stop?");
  }

  m_client->disconnect();
  m_client.reset();
  m_context.reset();

  m_eventBus->pushEvent(std::make_unique<ServerStoppedEvent>());
}

auto TcpClient::trySend(std::vector<char> bytes) -> std::optional<MessageId>
{
  return m_client->trySend(bytes);
}

} // namespace net
