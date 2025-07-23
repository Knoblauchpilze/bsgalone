
#include "ResourceListMessage.hh"
#include "DataSerialization.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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

auto ResourceListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_resourcesData.size());

  for (const auto &resourceData : m_resourcesData)
  {
    serializeResourceData(out, resourceData);
  }

  return out;
}

bool ResourceListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  std::size_t count;
  ok &= core::deserialize(in, count);

  m_resourcesData.clear();

  for (std::size_t id = 0u; id < count; ++id)
  {
    ResourceData data;

    ok &= deserializeResourceData(in, data);

    m_resourcesData.emplace_back(data);
  }

  return ok;
}

auto ResourceListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<ResourceListMessage>(m_resourcesData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
