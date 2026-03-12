
#include "SlotComponentMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

SlotComponentMessage::SlotComponentMessage()
  : ComponentUpdatedMessage(MessageType::SLOT_COMPONENT_UPDATED)
{}

SlotComponentMessage::SlotComponentMessage(
  const Uuid playerDbId,

  const Uuid shipDbId,
  const Uuid slotDbId,
  const std::optional<chrono::TickDuration> &elapsedSinceLastFired)
  : ComponentUpdatedMessage(MessageType::SLOT_COMPONENT_UPDATED,
                            shipDbId,
                            ComponentType::COMPUTER_SLOT)
  , m_playerDbId(playerDbId)
  , m_slotDbId(slotDbId)
  , m_elapsedSinceLastFired(elapsedSinceLastFired)
{}

auto SlotComponentMessage::getPlayerDbId() const -> int
{
  return m_playerDbId;
}

auto SlotComponentMessage::getSlotDbId() const -> int
{
  return m_slotDbId;
}

auto SlotComponentMessage::getElapsedSinceLastFired() const -> std::optional<chrono::TickDuration>
{
  return m_elapsedSinceLastFired;
}

auto SlotComponentMessage::clone() const -> IMessagePtr
{
  auto clone                     = std::make_unique<SlotComponentMessage>();
  clone->m_playerDbId            = m_playerDbId;
  clone->m_shipDbId              = m_shipDbId;
  clone->m_component             = m_component;
  clone->m_slotDbId              = m_slotDbId;
  clone->m_elapsedSinceLastFired = m_elapsedSinceLastFired;

  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const SlotComponentMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_shipDbId);
  ::core::serialize(out, message.m_component);
  ::core::serialize(out, message.m_slotDbId);
  ::core::serialize(out, message.m_elapsedSinceLastFired);

  return out;
}

auto operator>>(std::istream &in, SlotComponentMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_playerDbId);
  ::core::deserialize(in, message.m_shipDbId);
  ::core::deserialize(in, message.m_component);
  ::core::deserialize(in, message.m_slotDbId);
  ::core::deserialize(in, message.m_elapsedSinceLastFired);

  return in;
}

} // namespace bsgalone::core
