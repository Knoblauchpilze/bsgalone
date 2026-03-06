

#include "SlotMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

SlotMessage::SlotMessage()
  : AbstractPlayerMessage(MessageType::SLOT, Uuid{0}, Uuid{0})
{}

SlotMessage::SlotMessage(const Uuid playerDbId,
                         const Uuid systemDbId,
                         const Uuid shipDbId,
                         const Uuid slotDbId,
                         const Slot slotType)
  : AbstractPlayerMessage(MessageType::SLOT, playerDbId, systemDbId)
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
  ::core::serialize(out, m_messageType);
  ::core::serialize(out, m_playerDbId);
  ::core::serialize(out, m_systemDbId);

  ::core::serialize(out, m_shipDbId);
  ::core::serialize(out, m_slotDbId);
  ::core::serialize(out, m_slotType);

  return out;
}

bool SlotMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= ::core::deserialize(in, m_messageType);
  ok &= ::core::deserialize(in, m_playerDbId);
  ok &= ::core::deserialize(in, m_systemDbId);

  ok &= ::core::deserialize(in, m_shipDbId);
  ok &= ::core::deserialize(in, m_slotDbId);
  ok &= ::core::deserialize(in, m_slotType);

  return ok;
}

auto SlotMessage::clone() const -> IMessagePtr
{
  return std::make_unique<SlotMessage>(m_playerDbId,
                                       m_systemDbId,
                                       m_shipDbId,
                                       m_slotDbId,
                                       m_slotType);
}

auto SlotMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  SlotMessage message;

  if (!message.deserialize(in))
  {
    return {};
  }

  return message.clone();
}

} // namespace bsgalone::core
