
#include "PlayerResourceListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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

auto PlayerResourceListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_resourcesData);

  return out;
}

bool PlayerResourceListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_resourcesData);

  return ok;
}

auto PlayerResourceListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<PlayerResourceListMessage>(m_resourcesData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
