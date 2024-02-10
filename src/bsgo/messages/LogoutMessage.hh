
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class LogoutMessage : public NetworkMessage
{
  public:
  LogoutMessage();
  LogoutMessage(const Uuid playerDbId);
  ~LogoutMessage() override = default;

  auto getPlayerDbId() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_playerDbId{};
};

} // namespace bsgo
