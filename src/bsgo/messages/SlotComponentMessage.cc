
#include "SlotComponentMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

SlotComponentMessage::SlotComponentMessage()
  : ComponentUpdatedMessage(MessageType::SLOT_COMPONENT_UPDATED)
{}

SlotComponentMessage::SlotComponentMessage(const Uuid shipDbId,
                                           const Uuid slotDbId,
                                           const SlotComponent &component)
  : ComponentUpdatedMessage(MessageType::SLOT_COMPONENT_UPDATED, shipDbId, component.type())
  , m_slotDbId(slotDbId)
  , m_elapsedSinceLastFired(component.elapsedSinceLastFired())
{}

auto SlotComponentMessage::getSlotDbId() const -> int
{
  return m_slotDbId;
}

auto SlotComponentMessage::getElapsedSinceLastFired() const -> std::optional<utils::Duration>
{
  return m_elapsedSinceLastFired;
}

auto SlotComponentMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);

  utils::serialize(out, m_shipDbId);
  utils::serialize(out, m_component);
  utils::serialize(out, m_slotDbId);
  utils::serialize(out, m_elapsedSinceLastFired);

  return out;
}

bool SlotComponentMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);

  ok &= utils::deserialize(in, m_shipDbId);
  ok &= utils::deserialize(in, m_component);
  ok &= utils::deserialize(in, m_slotDbId);
  ok &= utils::deserialize(in, m_elapsedSinceLastFired);

  return ok;
}

auto SlotComponentMessage::clone() const -> IMessagePtr
{
  auto clone                     = std::make_unique<SlotComponentMessage>();
  clone->m_shipDbId              = m_shipDbId;
  clone->m_component             = m_component;
  clone->m_slotDbId              = m_slotDbId;
  clone->m_elapsedSinceLastFired = m_elapsedSinceLastFired;

  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
