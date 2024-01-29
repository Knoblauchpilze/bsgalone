
#include "SlotComponentMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

SlotComponentMessage::SlotComponentMessage()
  : ComponentUpdatedMessage(MessageType::SLOT_COMPONENT_UPDATED)
{}

SlotComponentMessage::SlotComponentMessage(const Uuid entityId,
                                           const int slotIndex,
                                           const SlotComponent &component)
  : ComponentUpdatedMessage(MessageType::SLOT_COMPONENT_UPDATED, entityId, component.type())
  , m_slotIndex(slotIndex)
  , m_elapsedSinceLastFired(component.elapsedSinceLastFired())
{}

auto SlotComponentMessage::getSlotIndex() const -> int
{
  return m_slotIndex;
}

auto SlotComponentMessage::getElapsedSinceLastFired() const -> std::optional<utils::Duration>
{
  return m_elapsedSinceLastFired;
}

auto SlotComponentMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);

  utils::serialize(out, m_entityId);
  utils::serialize(out, m_component);
  utils::serialize(out, m_slotIndex);
  utils::serialize(out, m_elapsedSinceLastFired);

  return out;
}

bool SlotComponentMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);

  ok &= utils::deserialize(in, m_entityId);
  ok &= utils::deserialize(in, m_component);
  ok &= utils::deserialize(in, m_slotIndex);
  ok &= utils::deserialize(in, m_elapsedSinceLastFired);

  return ok;
}

auto SlotComponentMessage::clone() const -> IMessagePtr
{
  auto clone                     = std::make_unique<SlotComponentMessage>();
  clone->m_entityId              = m_entityId;
  clone->m_component             = m_component;
  clone->m_slotIndex             = m_slotIndex;
  clone->m_elapsedSinceLastFired = m_elapsedSinceLastFired;

  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
