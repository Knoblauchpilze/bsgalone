
#include "LoadingFinishedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

LoadingFinishedMessage::LoadingFinishedMessage()
  : NetworkMessage(MessageType::LOADING_FINISHED)
{}

LoadingFinishedMessage::LoadingFinishedMessage(const LoadingTransition transition,
                                               const Uuid systemDbId)
  : NetworkMessage(MessageType::LOADING_FINISHED)
  , m_transition(transition)
  , m_systemDbId(systemDbId)
{}

LoadingFinishedMessage::LoadingFinishedMessage(const LoadingTransition transition,
                                               const Uuid systemDbId,
                                               const Uuid playerDbId)
  : NetworkMessage(MessageType::LOADING_FINISHED)
  , m_transition(transition)
  , m_systemDbId(systemDbId)
  , m_playerDbId(playerDbId)
{}

auto LoadingFinishedMessage::getTransition() const -> LoadingTransition
{
  return m_transition;
}

auto LoadingFinishedMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

auto LoadingFinishedMessage::tryGetPlayerDbId() const -> std::optional<Uuid>
{
  return m_playerDbId;
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

auto LoadingFinishedMessage::clone() const -> IMessagePtr
{
  std::unique_ptr<LoadingFinishedMessage> clone{};

  if (m_playerDbId)
  {
    clone = std::make_unique<LoadingFinishedMessage>(m_transition, m_systemDbId, *m_playerDbId);
  }
  else
  {
    clone = std::make_unique<LoadingFinishedMessage>(m_transition, m_systemDbId);
  }

  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
