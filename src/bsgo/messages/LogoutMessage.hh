
#pragma once

#include "Uuid.hh"
#include "ValidatableMessage.hh"

namespace bsgo {

class LogoutMessage : public ValidatableMessage
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
