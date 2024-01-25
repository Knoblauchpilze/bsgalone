
#include "NetworkMessage.hh"

namespace bsgo {

NetworkMessage::NetworkMessage(const MessageType &type)
  : AbstractMessage(type)
{}

void NetworkMessage::setClientId(const Uuid clientId)
{
  m_clientId = clientId;
}

auto NetworkMessage::getClientId() const -> Uuid
{
  if (!m_clientId)
  {
    throw std::invalid_argument("Expected non null client id");
  }
  return *m_clientId;
}

auto NetworkMessage::tryGetClientId() const -> std::optional<Uuid>
{
  return m_clientId;
}

} // namespace bsgo