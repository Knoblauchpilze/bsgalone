
#include "ShopDataMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

// TODO: Add tests for this class
// TODO: Add entries in the message parser/deseralizer
ShopDataMessage::ShopDataMessage()
  : IMessage(MessageType::SHOP_DATA)
{}

ShopDataMessage::ShopDataMessage(const Uuid playerDbId)
  : IMessage(MessageType::SHOP_DATA)
  , m_playerDbId(playerDbId)
{}

auto ShopDataMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto ShopDataMessage::clone() const -> IMessagePtr
{
  return std::make_unique<ShopDataMessage>(m_playerDbId);
}

auto ShopDataMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  ShopDataMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const ShopDataMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);

  return out;
}

} // namespace bsgalone::core
