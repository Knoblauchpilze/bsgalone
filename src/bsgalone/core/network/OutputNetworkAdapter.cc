
#include "OutputNetworkAdapter.hh"
#include <sstream>

namespace bsgalone::core {

OutputNetworkAdapter::OutputNetworkAdapter(net::INetworkServerShPtr server)
  : m_server(std::move(server))
{
  if (m_server == nullptr)
  {
    throw std::invalid_argument("Expected non null server");
  }
}

OutputNetworkAdapter::OutputNetworkAdapter(net::INetworkClientShPtr client)
  : m_client(std::move(client))
{
  if (m_client == nullptr)
  {
    throw std::invalid_argument("Expected non null client");
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
  std::ostringstream out{};
  out << message;

  const auto &rawMessage = out.str();
  return std::vector<char>(rawMessage.begin(), rawMessage.end());
}

} // namespace bsgalone::core
