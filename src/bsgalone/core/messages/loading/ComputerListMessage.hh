
#pragma once

#include "ComputerData.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class ComputerListMessage : public bsgalone::core::NetworkMessage
{
  public:
  ComputerListMessage();
  ComputerListMessage(const std::vector<ComputerData> &computersData);
  ~ComputerListMessage() override = default;

  auto getComputersData() const -> const std::vector<ComputerData> &;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> bsgalone::core::IMessagePtr override;

  private:
  std::vector<ComputerData> m_computersData{};
};

} // namespace bsgo
