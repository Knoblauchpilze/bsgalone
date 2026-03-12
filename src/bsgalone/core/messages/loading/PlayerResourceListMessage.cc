
#include "PlayerResourceListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

PlayerResourceListMessage::PlayerResourceListMessage()
  : NetworkMessage(MessageType::PLAYER_RESOURCE_LIST)
{}

PlayerResourceListMessage::PlayerResourceListMessage(
  const std::vector<PlayerResourceData> &resourcesData)
  : NetworkMessage(MessageType::PLAYER_RESOURCE_LIST)
  , m_resourcesData(resourcesData)
{}

auto PlayerResourceListMessage::getResourcesData() const -> const std::vector<PlayerResourceData> &
{
  return m_resourcesData;
}

auto PlayerResourceListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<PlayerResourceListMessage>(m_resourcesData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const PlayerResourceListMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_resourcesData);

  return out;
}

auto operator>>(std::istream &in, PlayerResourceListMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_resourcesData);

  return in;
}

} // namespace bsgalone::core
