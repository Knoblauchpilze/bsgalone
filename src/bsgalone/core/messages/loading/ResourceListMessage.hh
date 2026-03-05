
#pragma once

#include "NetworkMessage.hh"
#include "ResourceData.hh"

namespace bsgo {

class ResourceListMessage : public bsgalone::core::NetworkMessage
{
  public:
  ResourceListMessage();
  ResourceListMessage(const std::vector<ResourceData> &resourcesData);
  ~ResourceListMessage() override = default;

  auto getResourcesData() const -> const std::vector<ResourceData> &;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> bsgalone::core::IMessagePtr override;

  private:
  std::vector<ResourceData> m_resourcesData{};
};

} // namespace bsgo
