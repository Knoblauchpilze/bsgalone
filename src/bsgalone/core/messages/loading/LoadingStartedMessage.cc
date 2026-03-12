
#include "LoadingStartedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

LoadingStartedMessage::LoadingStartedMessage()
  : NetworkMessage(MessageType::LOADING_STARTED)
{}

LoadingStartedMessage::LoadingStartedMessage(const LoadingTransition transition,
                                             const Uuid playerDbId)
  : NetworkMessage(MessageType::LOADING_STARTED)
  , m_transition(transition)
  , m_playerDbId(playerDbId)
{}

auto LoadingStartedMessage::getTransition() const -> LoadingTransition
{
  return m_transition;
}

auto LoadingStartedMessage::getSystemDbId() const -> Uuid
{
  if (!m_systemDbId)
  {
    throw std::runtime_error("Expected system db id to be defined but it was not");
  }
  return *m_systemDbId;
}

auto LoadingStartedMessage::tryGetSystemDbId() const -> std::optional<Uuid>
{
  return m_systemDbId;
}

auto LoadingStartedMessage::getPlayerDbId() const -> Uuid
{
  if (!m_playerDbId)
  {
    throw std::runtime_error("Expected player db id to be defined but it was not");
  }
  return *m_playerDbId;
}

void LoadingStartedMessage::setSystemDbId(const Uuid systemDbId)
{
  m_systemDbId = systemDbId;
}

auto LoadingStartedMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<LoadingStartedMessage>(m_transition, *m_playerDbId);
  if (m_systemDbId)
  {
    clone->setSystemDbId(*m_systemDbId);
  }

  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const LoadingStartedMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_transition);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_playerDbId);

  return out;
}

auto operator>>(std::istream &in, LoadingStartedMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_transition);
  ::core::deserialize(in, message.m_systemDbId);
  ::core::deserialize(in, message.m_playerDbId);

  return in;
}

} // namespace bsgalone::core
