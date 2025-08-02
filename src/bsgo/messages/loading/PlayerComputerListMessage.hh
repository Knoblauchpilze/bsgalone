
#pragma once

#include "NetworkMessage.hh"
#include "PlayerComputerData.hh"
#include "Uuid.hh"

namespace bsgo {

class PlayerComputerListMessage : public NetworkMessage
{
  public:
  PlayerComputerListMessage();
  PlayerComputerListMessage(const std::vector<PlayerComputerData> &computersData);
  ~PlayerComputerListMessage() override = default;

  auto getComputersData() const -> const std::vector<PlayerComputerData> &;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  std::vector<PlayerComputerData> m_computersData{};
};

} // namespace bsgo
