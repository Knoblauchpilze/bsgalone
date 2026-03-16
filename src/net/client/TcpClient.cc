
#include "TcpClient.hh"
#include "ServerStartedEvent.hh"
#include "ServerStoppedEvent.hh"

namespace net {

TcpClient::TcpClient(INetworkEventQueueWPtr eventBus)
  : core::CoreObject("tcp")
  , m_eventBus(eventBus)
{
  setService("net");
  addModule("client");
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

  auto eventBus = m_eventBus.lock();
  if (!eventBus)
  {
    error("Cannot connect client, event bus is no longer available");
  }
  eventBus->pushEvent(std::make_unique<ServerStartedEvent>());
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

  auto eventBus = m_eventBus.lock();
  if (!eventBus)
  {
    error("Cannot disconnect client, event bus is no longer available");
  }
  eventBus->pushEvent(std::make_unique<ServerStoppedEvent>());
}

auto TcpClient::trySend(std::vector<char> bytes) -> std::optional<MessageId>
{
  return m_client->trySend(bytes);
}

} // namespace net
