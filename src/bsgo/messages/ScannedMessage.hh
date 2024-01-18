
#pragma once

#include "AbstractMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class ScannedMessage : public AbstractMessage
{
  public:
  ScannedMessage();
  ScannedMessage(const Uuid &asteroidEntityId);
  ~ScannedMessage() override = default;

  auto asteroidEntityId() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  private:
  Uuid m_asteroidEntityId{};
};

} // namespace bsgo
