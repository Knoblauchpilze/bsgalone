
#include "PurchaseMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

PurchaseMessage::PurchaseMessage()
  : AbstractPlayerMessage(MessageType::PURCHASE, bsgo::Uuid{0}, bsgo::Uuid{0})
{}

PurchaseMessage::PurchaseMessage(const bsgo::Uuid playerDbId,
                                 const bsgo::Uuid systemDbId,
                                 const Item &itemType,
                                 const bsgo::Uuid itemDbId)
  : AbstractPlayerMessage(MessageType::PURCHASE, playerDbId, systemDbId)
  , m_itemType(itemType)
  , m_itemDbId(itemDbId)
{}

auto PurchaseMessage::getItemType() const -> Item
{
  return m_itemType;
}

auto PurchaseMessage::getItemDbId() const -> bsgo::Uuid
{
  return m_itemDbId;
}

auto PurchaseMessage::serialize(std::ostream &out) const -> std::ostream &
{
  ::core::serialize(out, m_messageType);
  ::core::serialize(out, m_playerDbId);
  ::core::serialize(out, m_systemDbId);

  ::core::serialize(out, m_itemType);
  ::core::serialize(out, m_itemDbId);

  return out;
}

bool PurchaseMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= ::core::deserialize(in, m_messageType);
  ok &= ::core::deserialize(in, m_playerDbId);
  ok &= ::core::deserialize(in, m_systemDbId);

  ok &= ::core::deserialize(in, m_itemType);
  ok &= ::core::deserialize(in, m_itemDbId);

  return ok;
}

auto PurchaseMessage::clone() const -> IMessagePtr
{
  return std::make_unique<PurchaseMessage>(m_playerDbId, m_systemDbId, m_itemType, m_itemDbId);
}

auto PurchaseMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  PurchaseMessage message;

  if (!message.deserialize(in))
  {
    return {};
  }

  return message.clone();
}

} // namespace bsgalone::core
