
#include "SystemListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

SystemListMessage::SystemListMessage()
  : AbstractPlayerMessage(MessageType::SYSTEM_LIST, Uuid{0}, Uuid{0})
{}

SystemListMessage::SystemListMessage(const Uuid playerDbId,
                                     const Uuid systemDbId,
                                     const std::vector<SystemData> &systemsData)
  : AbstractPlayerMessage(MessageType::SYSTEM_LIST, playerDbId, systemDbId)
  , m_systemsData(systemsData)
{}

auto SystemListMessage::getSystemsData() const -> const std::vector<SystemData> &
{
  return m_systemsData;
}

auto SystemListMessage::clone() const -> IMessagePtr
{
  return std::make_unique<SystemListMessage>(m_playerDbId, m_systemDbId, m_systemsData);
}

auto SystemListMessage::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  SystemListMessage message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);
  ok &= ::core::deserialize(in, message.m_systemDbId);
  ok &= ::core::deserialize(in, message.m_systemsData);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const SystemListMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_systemsData);

  return out;
}

} // namespace bsgalone::core
