
#include "HangarDataMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

// TODO: Add tests for this class
// TODO: Add entries in the message parser/deseralizer
HangarDataMessage::HangarDataMessage()
  : IMessage(MessageType::HANGAR_DATA)
{}

HangarDataMessage::HangarDataMessage(const Uuid playerDbId)
  : IMessage(MessageType::HANGAR_DATA)
  , m_playerDbId(playerDbId)
{}

auto HangarDataMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto HangarDataMessage::clone() const -> IMessagePtr
{
  return std::make_unique<HangarDataMessage>(m_playerDbId);
}

auto HangarDataMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  HangarDataMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const HangarDataMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);

  return out;
}

} // namespace bsgalone::core
