

#pragma once

#include "IComponent.hh"
#include "Uuid.hh"

namespace bsgo {

class PlayerComponent : public IComponent
{
  public:
  PlayerComponent(const Uuid player);
  ~PlayerComponent() override = default;

  auto player() const -> Uuid;

  void update(const float elapsedSeconds) override;

  private:
  Uuid m_player;
};

using PlayerComponentShPtr = std::shared_ptr<PlayerComponent>;

} // namespace bsgo
