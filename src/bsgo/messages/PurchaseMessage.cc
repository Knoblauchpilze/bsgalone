
#include "PurchaseMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

PurchaseMessage::PurchaseMessage(const Uuid &playerDbId,
                                 const Item &type,
                                 const Uuid &itemDbId,
                                 const PurchaseState &state)
  : AbstractMessage(MessageType::PURCHASE)
  , m_playerDbId(playerDbId)
  , m_type(type)
  , m_itemDbId(itemDbId)
  , m_state(state)
{}

auto PurchaseMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto PurchaseMessage::getItemType() const -> Item
{
  return m_type;
}

auto PurchaseMessage::getItemDbId() const -> Uuid
{
  return m_itemDbId;
}

auto PurchaseMessage::getPurchaseState() const -> PurchaseState
{
  return m_state;
}

auto PurchaseMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);

  utils::serialize(out, m_playerDbId);
  utils::serialize(out, m_type);
  utils::serialize(out, m_itemDbId);
  utils::serialize(out, m_state);

  return out;
}

auto PurchaseMessage::deserialize(std::istream &in) -> std::istream &
{
  utils::deserialize(in, m_messageType);

  utils::deserialize(in, m_playerDbId);
  utils::deserialize(in, m_type);
  utils::deserialize(in, m_itemDbId);
  utils::deserialize(in, m_state);

  return in;
}

} // namespace bsgo
