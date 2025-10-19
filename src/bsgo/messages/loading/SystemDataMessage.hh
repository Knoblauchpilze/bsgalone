
#pragma once

#include "NetworkMessage.hh"
#include "SystemTickData.hh"

namespace bsgo {

class SystemDataMessage : public NetworkMessage
{
  public:
  SystemDataMessage();
  SystemDataMessage(const SystemTickData &tickData);
  ~SystemDataMessage() override = default;

  auto getTickData() const -> SystemTickData;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  SystemTickData m_tickData{};
};

} // namespace bsgo
