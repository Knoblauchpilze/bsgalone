
#include "JumpMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

JumpMessage::JumpMessage()
  : NetworkMessage(MessageType::JUMP)
{}

JumpMessage::JumpMessage(const Uuid shipDbId, const Uuid playerDbId)
  : NetworkMessage(MessageType::JUMP)
  , m_shipDbId(shipDbId)
  , m_playerDbId(playerDbId)
{}

JumpMessage::JumpMessage(const Uuid shipDbId,
                         const Uuid playerDbId,
                         const Uuid sourceSystemDbId,
                         const Uuid destinationSystemDbId)
  : NetworkMessage(MessageType::JUMP)
  , m_shipDbId(shipDbId)
  , m_playerDbId(playerDbId)
  , m_sourceSystemDbId(sourceSystemDbId)
  , m_destinationSystemDbId(destinationSystemDbId)
{}

auto JumpMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto JumpMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto JumpMessage::tryGetSourceSystemDbId() const -> std::optional<Uuid>
{
  return m_sourceSystemDbId;
}

auto JumpMessage::getSourceSystemDbId() const -> Uuid
{
  if (!m_sourceSystemDbId)
  {
    error("Expected source system db id to be defined but it was not");
  }
  return *m_sourceSystemDbId;
}

auto JumpMessage::tryGetDestinationSystemDbId() const -> std::optional<Uuid>
{
  return m_destinationSystemDbId;
}

auto JumpMessage::getDestinationSystemDbId() const -> Uuid
{
  if (!m_destinationSystemDbId)
  {
    error("Expected destination system db id to be defined but it was not");
  }
  return *m_destinationSystemDbId;
}

auto JumpMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_shipDbId);
  core::serialize(out, m_playerDbId);
  core::serialize(out, m_sourceSystemDbId);
  core::serialize(out, m_destinationSystemDbId);

  return out;
}

bool JumpMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_shipDbId);
  ok &= core::deserialize(in, m_playerDbId);
  ok &= core::deserialize(in, m_sourceSystemDbId);
  ok &= core::deserialize(in, m_destinationSystemDbId);

  return ok;
}

auto JumpMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone                     = std::make_unique<JumpMessage>(m_shipDbId, m_playerDbId);
  clone->m_sourceSystemDbId      = m_sourceSystemDbId;
  clone->m_destinationSystemDbId = m_destinationSystemDbId;
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
