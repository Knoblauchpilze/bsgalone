
#include "AiBehaviorSyncMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

AiBehaviorSyncMessage::AiBehaviorSyncMessage()
  : NetworkMessage(MessageType::AI_BEHAVIOR_SYNC)
{}

AiBehaviorSyncMessage::AiBehaviorSyncMessage(const Uuid shipDbId, const int targetIndex)
  : NetworkMessage(MessageType::AI_BEHAVIOR_SYNC)
  , m_shipDbId(shipDbId)
  , m_targetIndex(targetIndex)
{}

auto AiBehaviorSyncMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto AiBehaviorSyncMessage::getTargetIndex() const -> int
{
  return m_targetIndex;
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
  auto clone = std::make_unique<AiBehaviorSyncMessage>(m_shipDbId, m_targetIndex);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
