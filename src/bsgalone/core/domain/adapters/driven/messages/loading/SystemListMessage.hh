
#pragma once

#include "NetworkMessage.hh"
#include "SystemData.hh"

namespace bsgalone::core {

class SystemListMessage : public NetworkMessage
{
  public:
  SystemListMessage();
  SystemListMessage(const std::vector<SystemData> &systemsData);
  ~SystemListMessage() override = default;

  auto getSystemsData() const -> const std::vector<SystemData> &;

  auto clone() const -> IMessagePtr override;

  private:
  std::vector<SystemData> m_systemsData{};

  friend auto operator<<(std::ostream &out, const SystemListMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, SystemListMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const SystemListMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, SystemListMessage &message) -> std::istream &;

} // namespace bsgalone::core
