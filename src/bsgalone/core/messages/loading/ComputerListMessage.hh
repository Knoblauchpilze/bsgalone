
#pragma once

#include "ComputerData.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class ComputerListMessage : public NetworkMessage
{
  public:
  ComputerListMessage();
  ComputerListMessage(const std::vector<ComputerData> &computersData);
  ~ComputerListMessage() override = default;

  auto getComputersData() const -> const std::vector<ComputerData> &;

  auto clone() const -> IMessagePtr override;

  private:
  std::vector<ComputerData> m_computersData{};

  friend auto operator<<(std::ostream &out, const ComputerListMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, ComputerListMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const ComputerListMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, ComputerListMessage &message) -> std::istream &;

} // namespace bsgalone::core
