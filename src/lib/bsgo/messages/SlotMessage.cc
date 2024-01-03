

#include "SlotMessage.hh"

namespace bsgo {

SlotMessage::SlotMessage(const Uuid &shipEntityId,
                         const int slotIndex,
                         const Slot &slotType,
                         const SlotState &state)
  : AbstractMessage(MessageType::SLOT)
  , m_shipEntityId(shipEntityId)
  , m_slotIndex(slotIndex)
  , m_slotType(slotType)
  , m_state(state)
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

auto SlotMessage::getSlotState() const -> SlotState
{
  return m_state;
}

} // namespace bsgo
