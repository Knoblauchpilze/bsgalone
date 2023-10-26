
#include "PlayerComponent.hh"

namespace bsgo {

PlayerComponent::PlayerComponent(const Uuid player)
  : IComponent("player")
  , m_player(player)
{}

auto PlayerComponent::player() const -> Uuid
{
  return m_player;
}

void PlayerComponent::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
