
#include "LoadingFinishedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

LoadingFinishedMessage::LoadingFinishedMessage()
  : NetworkMessage(MessageType::LOADING_FINISHED)
{}

LoadingFinishedMessage::LoadingFinishedMessage(const Uuid systemDbId, const Uuid playerDbId)
  : NetworkMessage(MessageType::LOADING_FINISHED)
  , m_systemDbId(systemDbId)
  , m_playerDbId(playerDbId)
{}

auto LoadingFinishedMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

auto LoadingFinishedMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto LoadingFinishedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_systemDbId);
  core::serialize(out, m_playerDbId);

  return out;
}

bool LoadingFinishedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_systemDbId);
  ok &= core::deserialize(in, m_playerDbId);

  return ok;
}

auto LoadingFinishedMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<LoadingFinishedMessage>(m_systemDbId, m_playerDbId);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
