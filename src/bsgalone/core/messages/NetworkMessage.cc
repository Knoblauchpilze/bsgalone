
#include "NetworkMessage.hh"

namespace bsgalone::core {

NetworkMessage::NetworkMessage(const MessageType &type)
  : AbstractMessage(type)
{}

void NetworkMessage::setClientId(const bsgo::Uuid clientId)
{
  m_clientId = clientId;
}

auto NetworkMessage::getClientId() const -> bsgo::Uuid
{
  if (!m_clientId)
  {
    throw std::invalid_argument("Expected non null client id for " + str(type()));
  }
  return *m_clientId;
}

auto NetworkMessage::tryGetClientId() const -> std::optional<bsgo::Uuid>
{
  return m_clientId;
}

void NetworkMessage::copyClientIdIfDefined(const NetworkMessage &source)
{
  const auto maybeClientId = source.tryGetClientId();
  if (maybeClientId)
  {
    m_clientId = maybeClientId;
  }
}

} // namespace bsgalone::core
