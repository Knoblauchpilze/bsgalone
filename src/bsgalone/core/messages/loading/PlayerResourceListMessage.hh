
#pragma once

#include "NetworkMessage.hh"
#include "PlayerResourceData.hh"

namespace bsgalone::core {

class PlayerResourceListMessage : public NetworkMessage
{
  public:
  PlayerResourceListMessage();
  PlayerResourceListMessage(const std::vector<PlayerResourceData> &resourcesData);
  ~PlayerResourceListMessage() override = default;

  auto getResourcesData() const -> const std::vector<PlayerResourceData> &;

  auto clone() const -> IMessagePtr override;

  private:
  std::vector<PlayerResourceData> m_resourcesData{};

  friend auto operator<<(std::ostream &out, const PlayerResourceListMessage &message)
    -> std::ostream &;
  friend auto operator>>(std::istream &in, PlayerResourceListMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const PlayerResourceListMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, PlayerResourceListMessage &message) -> std::istream &;

} // namespace bsgalone::core
