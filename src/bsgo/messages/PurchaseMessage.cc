
#include "PurchaseMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

PurchaseMessage::PurchaseMessage()
  : NetworkMessage(MessageType::PURCHASE)
{}

PurchaseMessage::PurchaseMessage(const Uuid playerDbId, const Item &itemType, const Uuid itemDbId)
  : NetworkMessage(MessageType::PURCHASE)
  , m_playerDbId(playerDbId)
  , m_itemType(itemType)
  , m_itemDbId(itemDbId)
{}

auto PurchaseMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto PurchaseMessage::getItemType() const -> Item
{
  return m_itemType;
}

auto PurchaseMessage::getItemDbId() const -> Uuid
{
  return m_itemDbId;
}

auto PurchaseMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_playerDbId);
  core::serialize(out, m_itemType);
  core::serialize(out, m_itemDbId);

  return out;
}

bool PurchaseMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_playerDbId);
  ok &= core::deserialize(in, m_itemType);
  ok &= core::deserialize(in, m_itemDbId);

  return ok;
}

auto PurchaseMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<PurchaseMessage>(m_playerDbId, m_itemType, m_itemDbId);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
