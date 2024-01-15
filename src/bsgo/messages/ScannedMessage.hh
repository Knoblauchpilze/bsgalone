
#pragma once

#include "AbstractMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class ScannedMessage : public AbstractMessage
{
  public:
  ScannedMessage(const Uuid &asteroidEntityId);
  ~ScannedMessage() override = default;

  auto asteroidEntityId() const -> Uuid;

  auto operator<<(std::ostream &out) const -> std::ostream & override;
  auto operator>>(std::istream &in) -> std::istream & override;

  private:
  Uuid m_asteroidEntityId{};
};

} // namespace bsgo
