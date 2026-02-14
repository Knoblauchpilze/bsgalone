
#include "SystemListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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

auto SystemListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_systemsData);

  return out;
}

bool SystemListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_systemsData);

  return ok;
}

auto SystemListMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<SystemListMessage>(m_systemsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
