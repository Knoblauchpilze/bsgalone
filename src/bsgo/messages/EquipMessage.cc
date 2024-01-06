
#include "EquipMessage.hh"

namespace bsgo {

EquipMessage::EquipMessage(const EquipType &action,
                           const Uuid &shipDbId,
                           const Item &type,
                           const Uuid &itemDbId,
                           const EquipState &state)
  : AbstractMessage(MessageType::EQUIP)
  , m_action(action)
  , m_shipDbId(shipDbId)
  , m_type(type)
  , m_itemDbId(itemDbId)
  , m_state(state)
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
  return m_type;
}

auto EquipMessage::getItemDbId() const -> Uuid
{
  return m_itemDbId;
}

auto EquipMessage::getEquipState() const -> EquipState
{
  return m_state;
}

} // namespace bsgo
