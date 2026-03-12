
#pragma once

#include "NetworkMessage.hh"
#include "SystemTickData.hh"

namespace bsgalone::core {

class SystemDataMessage : public NetworkMessage
{
  public:
  SystemDataMessage();
  SystemDataMessage(const SystemTickData &tickData);
  ~SystemDataMessage() override = default;

  auto getTickData() const -> SystemTickData;

  auto clone() const -> IMessagePtr override;

  private:
  SystemTickData m_tickData{};

  friend auto operator<<(std::ostream &out, const SystemDataMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, SystemDataMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const SystemDataMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, SystemDataMessage &message) -> std::istream &;

} // namespace bsgalone::core
