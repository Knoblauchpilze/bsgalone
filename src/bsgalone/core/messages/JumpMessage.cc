
#include "JumpMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

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
    throw std::runtime_error("Expected source system db id to be defined but it was not");
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
    throw std::runtime_error("Expected destination system db id to be defined but it was not");
  }
  return *m_destinationSystemDbId;
}

auto JumpMessage::clone() const -> IMessagePtr
{
  auto clone                     = std::make_unique<JumpMessage>(m_shipDbId, m_playerDbId);
  clone->m_sourceSystemDbId      = m_sourceSystemDbId;
  clone->m_destinationSystemDbId = m_destinationSystemDbId;
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const JumpMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_shipDbId);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_sourceSystemDbId);
  ::core::serialize(out, message.m_destinationSystemDbId);

  return out;
}

auto operator>>(std::istream &in, JumpMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_shipDbId);
  ::core::deserialize(in, message.m_playerDbId);
  ::core::deserialize(in, message.m_sourceSystemDbId);
  ::core::deserialize(in, message.m_destinationSystemDbId);

  return in;
}

} // namespace bsgalone::core
