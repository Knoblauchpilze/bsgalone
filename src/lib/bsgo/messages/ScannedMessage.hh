
#pragma once

#include "SystemMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class ScannedMessage : public SystemMessage
{
  public:
  ScannedMessage(const Uuid &asteroidDbId);
  ~ScannedMessage() override = default;

  auto asteroidDbId() const -> Uuid;

  private:
  Uuid m_asteroidDbId{};
};

} // namespace bsgo
