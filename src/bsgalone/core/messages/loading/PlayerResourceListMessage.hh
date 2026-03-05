
#pragma once

#include "NetworkMessage.hh"
#include "PlayerResourceData.hh"

namespace bsgo {

class PlayerResourceListMessage : public bsgalone::core::NetworkMessage
{
  public:
  PlayerResourceListMessage();
  PlayerResourceListMessage(const std::vector<PlayerResourceData> &resourcesData);
  ~PlayerResourceListMessage() override = default;

  auto getResourcesData() const -> const std::vector<PlayerResourceData> &;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> bsgalone::core::IMessagePtr override;

  private:
  std::vector<PlayerResourceData> m_resourcesData{};
};

} // namespace bsgo
