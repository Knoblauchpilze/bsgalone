

#include "SlotMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

SlotMessage::SlotMessage()
  : ValidatableMessage(MessageType::SLOT)
{}

SlotMessage::SlotMessage(const Uuid &shipEntityId, const int slotIndex, const Slot &slotType)
  : ValidatableMessage(MessageType::SLOT)
  , m_shipEntityId(shipEntityId)
  , m_slotIndex(slotIndex)
  , m_slotType(slotType)
{}

auto SlotMessage::getShipEntityId() const -> Uuid
{
  return m_shipEntityId;
}

auto SlotMessage::getSlotIndex() const -> int
{
  return m_slotIndex;
}

auto SlotMessage::getSlotType() const -> Slot
{
  return m_slotType;
}

auto SlotMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_shipEntityId);
  utils::serialize(out, m_slotIndex);
  utils::serialize(out, m_slotType);

  return out;
}

bool SlotMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_shipEntityId);
  ok &= utils::deserialize(in, m_slotIndex);
  ok &= utils::deserialize(in, m_slotType);

  return ok;
}

} // namespace bsgo
