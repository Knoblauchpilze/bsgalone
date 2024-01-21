
#include "PurchaseMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

PurchaseMessage::PurchaseMessage()
  : ValidatableMessage(MessageType::PURCHASE)
{}

PurchaseMessage::PurchaseMessage(const Uuid &playerDbId, const Item &type, const Uuid &itemDbId)
  : ValidatableMessage(MessageType::PURCHASE)
  , m_playerDbId(playerDbId)
  , m_type(type)
  , m_itemDbId(itemDbId)
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

auto PurchaseMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_validated);

  utils::serialize(out, m_playerDbId);
  utils::serialize(out, m_type);
  utils::serialize(out, m_itemDbId);

  return out;
}

bool PurchaseMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_validated);

  ok &= utils::deserialize(in, m_playerDbId);
  ok &= utils::deserialize(in, m_type);
  ok &= utils::deserialize(in, m_itemDbId);

  return ok;
}

} // namespace bsgo
