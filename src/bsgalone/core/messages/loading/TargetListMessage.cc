
#include "TargetListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

TargetListMessage::TargetListMessage()
  : NetworkMessage(MessageType::TARGET_LIST)
{}

TargetListMessage::TargetListMessage(const Uuid systemDbId, const std::vector<Target> &targetsData)
  : NetworkMessage(MessageType::TARGET_LIST)
  , m_systemDbId(systemDbId)
  , m_targetsData(targetsData)
{}

auto TargetListMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

auto TargetListMessage::getTargetsData() const -> const std::vector<Target> &
{
  return m_targetsData;
}

auto TargetListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<TargetListMessage>(m_systemDbId, m_targetsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const TargetListMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_targetsData);

  return out;
}

auto operator>>(std::istream &in, TargetListMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_systemDbId);
  ::core::deserialize(in, message.m_targetsData);

  return in;
}

} // namespace bsgalone::core
