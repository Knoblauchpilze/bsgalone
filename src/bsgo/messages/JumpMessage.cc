
#include "JumpMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

JumpMessage::JumpMessage()
  : ValidatableMessage(MessageType::JUMP)
{}

JumpMessage::JumpMessage(const Uuid shipDbId, const Uuid playerDbId)
  : ValidatableMessage(MessageType::JUMP)
  , m_shipDbId(shipDbId)
  , m_playerDbId(playerDbId)
{}

JumpMessage::JumpMessage(const Uuid shipDbId,
                         const Uuid playerDbId,
                         const Uuid sourceSystemDbId,
                         const Uuid destinationSystemDbId)
  : ValidatableMessage(MessageType::JUMP)
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
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_shipDbId);
  utils::serialize(out, m_playerDbId);
  utils::serialize(out, m_sourceSystemDbId);
  utils::serialize(out, m_destinationSystemDbId);

  return out;
}

bool JumpMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_shipDbId);
  ok &= utils::deserialize(in, m_playerDbId);
  ok &= utils::deserialize(in, m_sourceSystemDbId);
  ok &= utils::deserialize(in, m_destinationSystemDbId);

  return ok;
}

auto JumpMessage::clone() const -> IMessagePtr
{
  auto clone                     = std::make_unique<JumpMessage>(m_shipDbId, m_playerDbId);
  clone->m_sourceSystemDbId      = m_sourceSystemDbId;
  clone->m_destinationSystemDbId = m_destinationSystemDbId;
  clone->copyClientIdIfDefined(*this);
  clone->validate(validated());

  return clone;
}

} // namespace bsgo
