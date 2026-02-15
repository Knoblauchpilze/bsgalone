
#include "ResourceListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

ResourceListMessage::ResourceListMessage()
  : NetworkMessage(bsgalone::core::MessageType::RESOURCE_LIST)
{}

ResourceListMessage::ResourceListMessage(const std::vector<ResourceData> &resourcesData)
  : NetworkMessage(bsgalone::core::MessageType::RESOURCE_LIST)
  , m_resourcesData(resourcesData)
{}

auto ResourceListMessage::getResourcesData() const -> const std::vector<ResourceData> &
{
  return m_resourcesData;
}

auto ResourceListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_resourcesData);

  return out;
}

bool ResourceListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_resourcesData);

  return ok;
}

auto ResourceListMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<ResourceListMessage>(m_resourcesData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
