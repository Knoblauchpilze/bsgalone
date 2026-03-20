
#include "PurchaseMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

PurchaseMessage::PurchaseMessage()
  : AbstractPlayerCommand(MessageType::PURCHASE, Uuid{0}, Uuid{0})
{}

PurchaseMessage::PurchaseMessage(const Uuid playerDbId,
                                 const Uuid systemDbId,
                                 const Item &itemType,
                                 const Uuid itemDbId)
  : AbstractPlayerCommand(MessageType::PURCHASE, playerDbId, systemDbId)
  , m_itemType(itemType)
  , m_itemDbId(itemDbId)
{}

auto PurchaseMessage::getItemType() const -> Item
{
  return m_itemType;
}

auto PurchaseMessage::getItemDbId() const -> Uuid
{
  return m_itemDbId;
}

auto PurchaseMessage::clone() const -> IMessagePtr
{
  return std::make_unique<PurchaseMessage>(m_playerDbId, m_systemDbId, m_itemType, m_itemDbId);
}

auto PurchaseMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  PurchaseMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);
  ok &= ::core::deserialize(in, message.m_systemDbId);
  ok &= ::core::deserialize(in, message.m_itemType);
  ok &= ::core::deserialize(in, message.m_itemDbId);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const PurchaseMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_itemType);
  ::core::serialize(out, message.m_itemDbId);

  return out;
}

} // namespace bsgalone::core
