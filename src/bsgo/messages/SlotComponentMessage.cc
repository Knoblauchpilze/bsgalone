
#include "SlotComponentMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

SlotComponentMessage::SlotComponentMessage()
  : ComponentUpdatedMessage(MessageType::SLOT_COMPONENT_UPDATED)
{}

SlotComponentMessage::SlotComponentMessage(const Uuid playerDbId,

                                           const Uuid shipDbId,
                                           const Uuid slotDbId,
                                           const std::optional<core::Duration> &elapsedSinceLastFired)
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

auto SlotComponentMessage::getElapsedSinceLastFired() const -> std::optional<core::Duration>
{
  return m_elapsedSinceLastFired;
}

auto SlotComponentMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_playerDbId);
  core::serialize(out, m_shipDbId);
  core::serialize(out, m_component);
  core::serialize(out, m_slotDbId);
  core::serialize(out, m_elapsedSinceLastFired);

  return out;
}

bool SlotComponentMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_playerDbId);
  ok &= core::deserialize(in, m_shipDbId);
  ok &= core::deserialize(in, m_component);
  ok &= core::deserialize(in, m_slotDbId);
  ok &= core::deserialize(in, m_elapsedSinceLastFired);

  return ok;
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

} // namespace bsgo
