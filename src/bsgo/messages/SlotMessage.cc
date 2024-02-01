

#include "SlotMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

SlotMessage::SlotMessage()
  : NetworkMessage(MessageType::SLOT)
{}

SlotMessage::SlotMessage(const Uuid shipDbId, const Uuid slotDbId, const Slot &slotType)
  : NetworkMessage(MessageType::SLOT)
  , m_shipDbId(shipDbId)
  , m_slotDbId(slotDbId)
  , m_slotType(slotType)
{}

auto SlotMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto SlotMessage::getSlotDbId() const -> Uuid
{
  return m_slotDbId;
}

auto SlotMessage::getSlotType() const -> Slot
{
  return m_slotType;
}

auto SlotMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);

  utils::serialize(out, m_shipDbId);
  utils::serialize(out, m_slotDbId);
  utils::serialize(out, m_slotType);

  return out;
}

bool SlotMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);

  ok &= utils::deserialize(in, m_shipDbId);
  ok &= utils::deserialize(in, m_slotDbId);
  ok &= utils::deserialize(in, m_slotType);

  return ok;
}

auto SlotMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<SlotMessage>(m_shipDbId, m_slotDbId, m_slotType);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
