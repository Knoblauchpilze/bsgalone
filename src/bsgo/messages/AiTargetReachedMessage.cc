
#include "AiTargetReachedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

AiTargetReachedMessage::AiTargetReachedMessage()
  : NetworkMessage(MessageType::AI_TARGET_REACHED)
{}

AiTargetReachedMessage::AiTargetReachedMessage(const Uuid shipDbId, const int targetIndex)
  : NetworkMessage(MessageType::AI_TARGET_REACHED)
  , m_shipDbId(shipDbId)
  , m_targetIndex(targetIndex)
{}

auto AiTargetReachedMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto AiTargetReachedMessage::getTargetIndex() const -> int
{
  return m_targetIndex;
}

auto AiTargetReachedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_shipDbId);
  core::serialize(out, m_targetIndex);

  return out;
}

bool AiTargetReachedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_shipDbId);
  ok &= core::deserialize(in, m_targetIndex);

  return ok;
}

auto AiTargetReachedMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<AiTargetReachedMessage>(m_shipDbId, m_targetIndex);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
