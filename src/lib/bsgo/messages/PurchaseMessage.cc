
#include "PurchaseMessage.hh"

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

} // namespace bsgo
