
#include "LogoutMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

LogoutMessage::LogoutMessage()
  : NetworkMessage(MessageType::LOGOUT)
{}

LogoutMessage::LogoutMessage(const Uuid playerDbId)
  : NetworkMessage(MessageType::LOGOUT)
  , m_playerDbId(playerDbId)
{}

auto LogoutMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto LogoutMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);

  utils::serialize(out, m_playerDbId);

  return out;
}

bool LogoutMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);

  ok &= utils::deserialize(in, m_playerDbId);

  return ok;
}

auto LogoutMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<LogoutMessage>(m_playerDbId);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
