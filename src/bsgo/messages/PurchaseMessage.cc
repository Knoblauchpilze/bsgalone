
#include "PurchaseMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

PurchaseMessage::PurchaseMessage()
  : AbstractMessage(MessageType::PURCHASE)
{}

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

bool PurchaseMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);

  ok &= utils::deserialize(in, m_playerDbId);
  ok &= utils::deserialize(in, m_type);
  ok &= utils::deserialize(in, m_itemDbId);
  ok &= utils::deserialize(in, m_state);

  return ok;
}

} // namespace bsgo
