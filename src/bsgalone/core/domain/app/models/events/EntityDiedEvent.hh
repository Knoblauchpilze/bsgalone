
#pragma once

#include "IGameEvent.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class EntityDiedEvent : public IGameEvent
{
  public:
  EntityDiedEvent(const Uuid entityId);
  ~EntityDiedEvent() override = default;

  auto entityId() const -> Uuid;

  auto clone() const -> IGameEventPtr override;

  private:
  Uuid m_entityId{};
};

} // namespace bsgalone::core
