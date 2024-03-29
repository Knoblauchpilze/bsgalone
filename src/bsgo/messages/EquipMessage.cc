
#include "EquipMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

EquipMessage::EquipMessage()
  : ValidatableMessage(MessageType::EQUIP)
{}

EquipMessage::EquipMessage(const EquipType &action,
                           const Uuid shipDbId,
                           const Item &itemType,
                           const Uuid itemDbId)
  : ValidatableMessage(MessageType::EQUIP)
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

auto EquipMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_action);
  utils::serialize(out, m_shipDbId);
  utils::serialize(out, m_itemType);
  utils::serialize(out, m_itemDbId);

  return out;
}

bool EquipMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_action);
  ok &= utils::deserialize(in, m_shipDbId);
  ok &= utils::deserialize(in, m_itemType);
  ok &= utils::deserialize(in, m_itemDbId);

  return ok;
}

auto EquipMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<EquipMessage>(m_action, m_shipDbId, m_itemType, m_itemDbId);
  clone->copyClientIdIfDefined(*this);
  clone->validate(validated());

  return clone;
}

} // namespace bsgo
