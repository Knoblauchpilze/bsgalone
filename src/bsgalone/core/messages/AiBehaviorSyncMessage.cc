
#include "AiBehaviorSyncMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

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

auto AiBehaviorSyncMessage::tryGetSystemDbId() const -> std::optional<Uuid>
{
  return m_systemDbId;
}

auto AiBehaviorSyncMessage::tryGetTargetIndex() const -> std::optional<int>
{
  return m_targetIndex;
}

void AiBehaviorSyncMessage::setSystemDbId(const Uuid systemDbId)
{
  m_systemDbId = systemDbId;
}

void AiBehaviorSyncMessage::setTargetIndex(const int targetIndex)
{
  m_targetIndex = targetIndex;
}

auto AiBehaviorSyncMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<AiBehaviorSyncMessage>(m_shipDbId);
  clone->copyClientIdIfDefined(*this);

  if (m_systemDbId)
  {
    clone->setSystemDbId(*m_systemDbId);
  }
  if (m_targetIndex)
  {
    clone->setTargetIndex(*m_targetIndex);
  }

  return clone;
}

auto operator<<(std::ostream &out, const AiBehaviorSyncMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_shipDbId);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_targetIndex);

  return out;
}

auto operator>>(std::istream &in, AiBehaviorSyncMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_shipDbId);
  ::core::deserialize(in, message.m_systemDbId);
  ::core::deserialize(in, message.m_targetIndex);

  return in;
}

} // namespace bsgalone::core
