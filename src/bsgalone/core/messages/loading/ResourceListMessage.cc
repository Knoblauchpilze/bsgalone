
#include "ResourceListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

ResourceListMessage::ResourceListMessage()
  : NetworkMessage(MessageType::RESOURCE_LIST)
{}

ResourceListMessage::ResourceListMessage(const std::vector<ResourceData> &resourcesData)
  : NetworkMessage(MessageType::RESOURCE_LIST)
  , m_resourcesData(resourcesData)
{}

auto ResourceListMessage::getResourcesData() const -> const std::vector<ResourceData> &
{
  return m_resourcesData;
}

auto ResourceListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<ResourceListMessage>(m_resourcesData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const ResourceListMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_resourcesData);

  return out;
}

auto operator>>(std::istream &in, ResourceListMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_resourcesData);

  return in;
}

} // namespace bsgalone::core
