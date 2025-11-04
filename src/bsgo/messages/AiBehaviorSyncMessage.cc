
#include "AiBehaviorSyncMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

AiBehaviorSyncMessage::AiBehaviorSyncMessage()
  : NetworkMessage(MessageType::AI_BEHAVIOR_SYNC)
{}

AiBehaviorSyncMessage::AiBehaviorSyncMessage(const Uuid shipDbId)
  : NetworkMessage(MessageType::AI_BEHAVIOR_SYNC)
  , m_shipDbId(shipDbId)
{}

auto AiBehaviorSyncMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto AiBehaviorSyncMessage::tryGetTargetIndex() const -> std::optional<int>
{
  return m_targetIndex;
}

void AiBehaviorSyncMessage::setTargetIndex(const int targetIndex)
{
  m_targetIndex = targetIndex;
}

auto AiBehaviorSyncMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_shipDbId);
  core::serialize(out, m_targetIndex);

  return out;
}

bool AiBehaviorSyncMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_shipDbId);
  ok &= core::deserialize(in, m_targetIndex);

  return ok;
}

auto AiBehaviorSyncMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<AiBehaviorSyncMessage>(m_shipDbId);
  clone->copyClientIdIfDefined(*this);

  if (m_targetIndex)
  {
    clone->setTargetIndex(*m_targetIndex);
  }

  return clone;
}

} // namespace bsgo
