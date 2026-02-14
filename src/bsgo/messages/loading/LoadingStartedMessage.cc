
#include "LoadingStartedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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
    error("Expected system db id to be defined but it was not");
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
    error("Expected player db id to be defined but it was not");
  }
  return *m_playerDbId;
}

void LoadingStartedMessage::setSystemDbId(const Uuid systemDbId)
{
  m_systemDbId = systemDbId;
}

auto LoadingStartedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_transition);
  core::serialize(out, m_systemDbId);
  core::serialize(out, m_playerDbId);

  return out;
}

bool LoadingStartedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_transition);
  ok &= core::deserialize(in, m_systemDbId);
  ok &= core::deserialize(in, m_playerDbId);

  return ok;
}

auto LoadingStartedMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<LoadingStartedMessage>(m_transition, *m_playerDbId);
  if (m_systemDbId)
  {
    clone->setSystemDbId(*m_systemDbId);
  }

  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
