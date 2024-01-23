

#pragma once

#include "Uuid.hh"
#include "ValidatableMessage.hh"

namespace bsgo {

class ConnectionMessage : public ValidatableMessage
{
  public:
  ConnectionMessage();
  ConnectionMessage(const Uuid &clientId);
  ~ConnectionMessage() override = default;

  auto getClientId() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  private:
  Uuid m_clientId{};
};

} // namespace bsgo
