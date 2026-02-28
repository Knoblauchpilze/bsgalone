
#pragma once

#include "IGameEvent.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class EntityDiedEvent : public IGameEvent
{
  public:
  EntityDiedEvent(const bsgo::Uuid entityId);
  ~EntityDiedEvent() override = default;

  auto entityId() const -> bsgo::Uuid;

  auto clone() const -> IGameEventPtr override;

  private:
  bsgo::Uuid m_entityId{};
};

} // namespace bsgalone::core
