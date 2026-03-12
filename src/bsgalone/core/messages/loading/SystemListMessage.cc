
#include "SystemListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

SystemListMessage::SystemListMessage()
  : NetworkMessage(MessageType::SYSTEM_LIST)
{}

SystemListMessage::SystemListMessage(const std::vector<SystemData> &systemsData)
  : NetworkMessage(MessageType::SYSTEM_LIST)
  , m_systemsData(systemsData)
{}

auto SystemListMessage::getSystemsData() const -> const std::vector<SystemData> &
{
  return m_systemsData;
}

auto SystemListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<SystemListMessage>(m_systemsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const SystemListMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_systemsData);

  return out;
}

auto operator>>(std::istream &in, SystemListMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_systemsData);

  return in;
}

} // namespace bsgalone::core
