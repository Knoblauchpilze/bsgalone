
#pragma once

#include "SystemMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class ScannedMessage : public SystemMessage
{
  public:
  ScannedMessage(const Uuid &asteroidEntityId);
  ~ScannedMessage() override = default;

  auto asteroidEntityId() const -> Uuid;

  private:
  Uuid m_asteroidEntityId{};
};

} // namespace bsgo
