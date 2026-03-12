
#pragma once

#include "NetworkMessage.hh"
#include "ResourceData.hh"

namespace bsgalone::core {

class ResourceListMessage : public NetworkMessage
{
  public:
  ResourceListMessage();
  ResourceListMessage(const std::vector<ResourceData> &resourcesData);
  ~ResourceListMessage() override = default;

  auto getResourcesData() const -> const std::vector<ResourceData> &;

  auto clone() const -> IMessagePtr override;

  private:
  std::vector<ResourceData> m_resourcesData{};

  friend auto operator<<(std::ostream &out, const ResourceListMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, ResourceListMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const ResourceListMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, ResourceListMessage &message) -> std::istream &;

} // namespace bsgalone::core
