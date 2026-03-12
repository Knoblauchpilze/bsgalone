
#pragma once

#include "NetworkMessage.hh"
#include "PlayerComputerData.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class PlayerComputerListMessage : public NetworkMessage
{
  public:
  PlayerComputerListMessage();
  PlayerComputerListMessage(const std::vector<PlayerComputerData> &computersData);
  ~PlayerComputerListMessage() override = default;

  auto getComputersData() const -> const std::vector<PlayerComputerData> &;

  auto clone() const -> IMessagePtr override;

  private:
  std::vector<PlayerComputerData> m_computersData{};

  friend auto operator<<(std::ostream &out, const PlayerComputerListMessage &message)
    -> std::ostream &;
  friend auto operator>>(std::istream &in, PlayerComputerListMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const PlayerComputerListMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, PlayerComputerListMessage &message) -> std::istream &;

} // namespace bsgalone::core
