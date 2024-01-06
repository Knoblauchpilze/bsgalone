
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

  private:
  Uuid m_asteroidEntityId{};
};

} // namespace bsgo
