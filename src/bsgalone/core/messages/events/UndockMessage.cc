
#include "UndockMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

UndockMessage::UndockMessage()
  : IMessage(MessageType::UNDOCK)
{}

UndockMessage::UndockMessage(const Uuid playerDbId)
  : IMessage(MessageType::UNDOCK)
  , m_playerDbId(playerDbId)
{}

auto UndockMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto UndockMessage::clone() const -> IMessagePtr
{
  return std::make_unique<UndockMessage>(m_playerDbId);
}

auto UndockMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  UndockMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const UndockMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);

  return out;
}

} // namespace bsgalone::core
