
#pragma once

#include "Uuid.hh"
#include "ValidatableMessage.hh"

namespace bsgo {

class ScannedMessage : public ValidatableMessage
{
  public:
  ScannedMessage();
  ScannedMessage(const Uuid asteroidDbId);
  ~ScannedMessage() override = default;

  auto getAsteroidDbId() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_asteroidDbId{};
};

} // namespace bsgo
