

#include "SlotMessage.hh"
#include <core_utils/SerializationUtils.hh>

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

auto SlotMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);

  utils::serialize(out, m_shipEntityId);
  utils::serialize(out, m_slotIndex);
  utils::serialize(out, m_slotType);
  utils::serialize(out, m_state);

  return out;
}

bool SlotMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);

  ok &= utils::deserialize(in, m_shipEntityId);
  ok &= utils::deserialize(in, m_slotIndex);
  ok &= utils::deserialize(in, m_slotType);
  ok &= utils::deserialize(in, m_state);

  return ok;
}

} // namespace bsgo
