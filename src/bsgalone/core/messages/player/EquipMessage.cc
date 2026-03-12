
#include "EquipMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

EquipMessage::EquipMessage()
  : AbstractPlayerMessage(MessageType::EQUIP, Uuid{0}, Uuid{0})
{}

EquipMessage::EquipMessage(const Uuid playerDbId,
                           const Uuid systemDbId,
                           const EquipType &action,
                           const Uuid shipDbId,
                           const Item &itemType,
                           const Uuid itemDbId)
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

auto EquipMessage::getShipDbId() const -> Uuid
{
  return m_shipDbId;
}

auto EquipMessage::getItemType() const -> Item
{
  return m_itemType;
}

auto EquipMessage::getItemDbId() const -> Uuid
{
  return m_itemDbId;
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

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);
  ok &= ::core::deserialize(in, message.m_systemDbId);
  ok &= ::core::deserialize(in, message.m_action);
  ok &= ::core::deserialize(in, message.m_shipDbId);
  ok &= ::core::deserialize(in, message.m_itemType);
  ok &= ::core::deserialize(in, message.m_itemDbId);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const EquipMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_action);
  ::core::serialize(out, message.m_shipDbId);
  ::core::serialize(out, message.m_itemType);
  ::core::serialize(out, message.m_itemDbId);

  return out;
}

} // namespace bsgalone::core
