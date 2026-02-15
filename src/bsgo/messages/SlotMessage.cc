

#include "SlotMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

SlotMessage::SlotMessage()
  : NetworkMessage(bsgalone::core::MessageType::SLOT)
{}

SlotMessage::SlotMessage(const Uuid shipDbId, const Uuid slotDbId, const Slot &slotType)
  : NetworkMessage(bsgalone::core::MessageType::SLOT)
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
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_shipDbId);
  core::serialize(out, m_slotDbId);
  core::serialize(out, m_slotType);

  return out;
}

bool SlotMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_shipDbId);
  ok &= core::deserialize(in, m_slotDbId);
  ok &= core::deserialize(in, m_slotType);

  return ok;
}

auto SlotMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<SlotMessage>(m_shipDbId, m_slotDbId, m_slotType);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
