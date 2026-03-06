
#pragma once

#include "IGameEvent.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class JumpCompletedEvent : public IGameEvent
{
  public:
  JumpCompletedEvent(const Uuid entityId);
  ~JumpCompletedEvent() override = default;

  auto entityId() const -> Uuid;

  auto clone() const -> IGameEventPtr override;

  private:
  Uuid m_entityId{};
};

} // namespace bsgalone::core
