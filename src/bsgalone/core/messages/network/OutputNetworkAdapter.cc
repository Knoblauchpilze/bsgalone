
#include "OutputNetworkAdapter.hh"
#include <sstream>

namespace bsgalone::core {

OutputNetworkAdapter::OutputNetworkAdapter(net::INetworkServerShPtr server,
                                           IMessageSerializerPtr serializer)
  : IOutputNetworkAdapter()
  , m_server(std::move(server))
  , m_serializer(std::move(serializer))
{
  if (m_server == nullptr)
  {
    throw std::invalid_argument("Expected non null server");
  }
  if (m_serializer == nullptr)
  {
    throw std::invalid_argument("Expected non null serializer");
  }
}

OutputNetworkAdapter::OutputNetworkAdapter(net::INetworkClientShPtr client,
                                           IMessageSerializerPtr serializer)
  : m_client(std::move(client))
  , m_serializer(std::move(serializer))
{
  if (m_client == nullptr)
  {
    throw std::invalid_argument("Expected non null client");
  }
  if (m_serializer == nullptr)
  {
    throw std::invalid_argument("Expected non null serializer");
  }
}

void OutputNetworkAdapter::sendMessage(const net::ClientId clientId, const IMessage &message)
{
  if (m_server == nullptr)
  {
    throw std::invalid_argument("Expected non null server");
  }

  m_server->trySend(clientId, serializeMessage(message));
}

void OutputNetworkAdapter::sendMessage(const IMessage &message)
{
  if (m_client == nullptr)
  {
    throw std::invalid_argument("Expected non null client");
  }

  m_client->trySend(serializeMessage(message));
}

auto OutputNetworkAdapter::serializeMessage(const IMessage &message) const -> std::vector<char>
{
  return m_serializer->serializeMessage(message);
}

} // namespace bsgalone::core
