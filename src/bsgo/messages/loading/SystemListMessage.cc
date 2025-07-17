
#include "SystemListMessage.hh"
#include "DataSerialization.hh"
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

  core::serialize(out, m_systemsData.size());

  for (const auto &systemData : m_systemsData)
  {
    serializeSystemData(out, systemData);
  }

  return out;
}

bool SystemListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  std::size_t count;
  ok &= core::deserialize(in, count);

  m_systemsData.clear();

  for (std::size_t id = 0u; id < count; ++id)
  {
    SystemData data;

    ok &= deserializeSystemData(in, data);

    m_systemsData.emplace_back(data);
  }

  return ok;
}

auto SystemListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<SystemListMessage>(m_systemsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
