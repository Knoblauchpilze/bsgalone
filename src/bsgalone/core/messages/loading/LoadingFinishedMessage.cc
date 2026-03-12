
#include "LoadingFinishedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

LoadingFinishedMessage::LoadingFinishedMessage()
  : NetworkMessage(MessageType::LOADING_FINISHED)
{}

LoadingFinishedMessage::LoadingFinishedMessage(const LoadingTransition transition,
                                               const Uuid playerDbId)
  : NetworkMessage(MessageType::LOADING_FINISHED)
  , m_transition(transition)
  , m_playerDbId(playerDbId)
{}

auto LoadingFinishedMessage::getTransition() const -> LoadingTransition
{
  return m_transition;
}

auto LoadingFinishedMessage::tryGetSystemDbId() const -> std::optional<Uuid>
{
  return m_systemDbId;
}

auto LoadingFinishedMessage::getPlayerDbId() const -> Uuid
{
  if (!m_playerDbId)
  {
    throw std::runtime_error("Expected player db id to be defined but it was not");
  }
  return *m_playerDbId;
}

void LoadingFinishedMessage::setSystemDbId(const Uuid systemDbId)
{
  m_systemDbId = systemDbId;
}

auto LoadingFinishedMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<LoadingFinishedMessage>(m_transition, *m_playerDbId);
  if (m_systemDbId)
  {
    clone->setSystemDbId(*m_systemDbId);
  }

  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const LoadingFinishedMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_transition);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_playerDbId);

  return out;
}

auto operator>>(std::istream &in, LoadingFinishedMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_transition);
  ::core::deserialize(in, message.m_systemDbId);
  ::core::deserialize(in, message.m_playerDbId);

  return in;
}

} // namespace bsgalone::core
