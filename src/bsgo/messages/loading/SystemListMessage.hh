
#pragma once

#include "NetworkMessage.hh"
#include "SystemData.hh"

namespace bsgo {

class SystemListMessage : public bsgalone::core::NetworkMessage
{
  public:
  SystemListMessage();
  SystemListMessage(const std::vector<SystemData> &systemsData);
  ~SystemListMessage() override = default;

  auto getSystemsData() const -> const std::vector<SystemData> &;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> bsgalone::core::IMessagePtr override;

  private:
  std::vector<SystemData> m_systemsData{};
};

} // namespace bsgo
