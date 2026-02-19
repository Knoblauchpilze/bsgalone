
#include "EquipMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

EquipMessage::EquipMessage()
  : AbstractPlayerMessage(MessageType::EQUIP, bsgo::Uuid{0}, bsgo::Uuid{0})
{}

EquipMessage::EquipMessage(const bsgo::Uuid playerDbId,
                           const bsgo::Uuid systemDbId,
                           const EquipType &action,
                           const bsgo::Uuid shipDbId,
                           const Item &itemType,
                           const bsgo::Uuid itemDbId)
  : AbstractPlayerMessage(MessageType::EQUIP, playerDbId, systemDbId)
  , m_action(action)
  , m_shipDbId(shipDbId)
  , m_itemType(itemType)
  , m_itemDbId(itemDbId)
{}

auto EquipMessage::getAction() const -> EquipType
{
  return m_action;
}

auto EquipMessage::getShipDbId() const -> bsgo::Uuid
{
  return m_shipDbId;
}

auto EquipMessage::getItemType() const -> Item
{
  return m_itemType;
}

auto EquipMessage::getItemDbId() const -> bsgo::Uuid
{
  return m_itemDbId;
}

auto EquipMessage::serialize(std::ostream &out) const -> std::ostream &
{
  ::core::serialize(out, m_messageType);
  ::core::serialize(out, m_playerDbId);
  ::core::serialize(out, m_systemDbId);

  ::core::serialize(out, m_action);
  ::core::serialize(out, m_shipDbId);
  ::core::serialize(out, m_itemType);
  ::core::serialize(out, m_itemDbId);

  return out;
}

bool EquipMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= ::core::deserialize(in, m_messageType);
  ok &= ::core::deserialize(in, m_playerDbId);
  ok &= ::core::deserialize(in, m_systemDbId);

  ok &= ::core::deserialize(in, m_action);
  ok &= ::core::deserialize(in, m_shipDbId);
  ok &= ::core::deserialize(in, m_itemType);
  ok &= ::core::deserialize(in, m_itemDbId);

  return ok;
}

auto EquipMessage::clone() const -> IMessagePtr
{
  return std::make_unique<EquipMessage>(m_playerDbId,
                                        m_systemDbId,
                                        m_action,
                                        m_shipDbId,
                                        m_itemType,
                                        m_itemDbId);
}

auto EquipMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  EquipMessage message;

  if (!message.deserialize(in))
  {
    return {};
  }

  return message.clone();
}

} // namespace bsgalone::core
