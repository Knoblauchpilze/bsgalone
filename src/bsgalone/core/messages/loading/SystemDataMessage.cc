
#include "SystemDataMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

SystemDataMessage::SystemDataMessage()
  : NetworkMessage(MessageType::SYSTEM_DATA)
{}

SystemDataMessage::SystemDataMessage(const SystemTickData &tickData)
  : NetworkMessage(MessageType::SYSTEM_DATA)
  , m_tickData(tickData)
{}

auto SystemDataMessage::getTickData() const -> SystemTickData
{
  return m_tickData;
}

auto SystemDataMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<SystemDataMessage>(m_tickData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const SystemDataMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_tickData);

  return out;
}

auto operator>>(std::istream &in, SystemDataMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_tickData);

  return in;
}

} // namespace bsgalone::core
