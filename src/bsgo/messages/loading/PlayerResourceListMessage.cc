
#include "PlayerResourceListMessage.hh"
#include "DataSerialization.hh"
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

  core::serialize(out, m_resourcesData.size());

  for (const auto &resourceData : m_resourcesData)
  {
    serializePlayerResourceData(out, resourceData);
  }

  return out;
}

bool PlayerResourceListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  std::size_t count;
  ok &= core::deserialize(in, count);

  m_resourcesData.clear();

  for (std::size_t id = 0u; id < count; ++id)
  {
    PlayerResourceData data;

    ok &= deserializePlayerResourceData(in, data);

    m_resourcesData.emplace_back(data);
  }

  return ok;
}

auto PlayerResourceListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<PlayerResourceListMessage>(m_resourcesData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
