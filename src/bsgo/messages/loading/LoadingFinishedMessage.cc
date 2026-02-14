
#include "LoadingFinishedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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
    error("Expected player db id to be defined but it was not");
  }
  return *m_playerDbId;
}

void LoadingFinishedMessage::setSystemDbId(const Uuid systemDbId)
{
  m_systemDbId = systemDbId;
}

auto LoadingFinishedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_transition);
  core::serialize(out, m_systemDbId);
  core::serialize(out, m_playerDbId);

  return out;
}

bool LoadingFinishedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_transition);
  ok &= core::deserialize(in, m_systemDbId);
  ok &= core::deserialize(in, m_playerDbId);

  return ok;
}

auto LoadingFinishedMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<LoadingFinishedMessage>(m_transition, *m_playerDbId);
  if (m_systemDbId)
  {
    clone->setSystemDbId(*m_systemDbId);
  }

  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
