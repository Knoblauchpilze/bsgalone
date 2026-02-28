
#pragma once

#include "IGameEvent.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class JumpCompletedEvent : public IGameEvent
{
  public:
  JumpCompletedEvent(const bsgo::Uuid entityId);
  ~JumpCompletedEvent() override = default;

  auto entityId() const -> bsgo::Uuid;

  auto clone() const -> IGameEventPtr override;

  private:
  bsgo::Uuid m_entityId{};
};

} // namespace bsgalone::core
