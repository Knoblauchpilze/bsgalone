
#include "EquipMessage.hh"
#include <core_utils/SerializationUtils.hh>

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

auto EquipMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);

  utils::serialize(out, m_action);
  out << m_shipDbId;
  utils::serialize(out, m_type);
  out << m_itemDbId;
  utils::serialize(out, m_state);

  return out;
}

auto EquipMessage::deserialize(std::istream &in) -> std::istream &
{
  utils::deserialize(in, m_messageType);

  utils::deserialize(in, m_action);
  in >> m_shipDbId;
  utils::deserialize(in, m_type);
  in >> m_itemDbId;
  utils::deserialize(in, m_state);

  return in;
}

} // namespace bsgo
