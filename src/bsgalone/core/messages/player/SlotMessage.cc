

#include "SlotMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

SlotMessage::SlotMessage()
  : AbstractPlayerCommand(MessageType::SLOT, Uuid{0}, Uuid{0})
{}

SlotMessage::SlotMessage(const Uuid playerDbId,
                         const Uuid systemDbId,
                         const Uuid shipDbId,
                         const Uuid slotDbId,
                         const Slot slotType)
  : AbstractPlayerCommand(MessageType::SLOT, playerDbId, systemDbId)
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

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);
  ok &= ::core::deserialize(in, message.m_systemDbId);
  ok &= ::core::deserialize(in, message.m_shipDbId);
  ok &= ::core::deserialize(in, message.m_slotDbId);
  ok &= ::core::deserialize(in, message.m_slotType);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const SlotMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_shipDbId);
  ::core::serialize(out, message.m_slotDbId);
  ::core::serialize(out, message.m_slotType);

  return out;
}

} // namespace bsgalone::core
