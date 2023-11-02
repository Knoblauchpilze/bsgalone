
#include "ComputerSlotComponent.hh"

namespace bsgo {

ComputerSlotComponent::ComputerSlotComponent(const PlayerComputer &computer)
  : SlotComponent("computer",
                  SlotComponentData{.offensive  = computer.offensive,
                                    .powerCost  = computer.powerCost,
                                    .range      = computer.range,
                                    .reloadTime = computer.reloadTime})
  , m_duration(computer.duration)
  , m_allowedTargets(computer.allowedTargets)
  , m_damageModifier(computer.damageModifier)
{}

auto ComputerSlotComponent::duration() const -> std::optional<utils::Duration>
{
  return m_duration;
}

auto ComputerSlotComponent::allowedTargets() const -> std::optional<std::unordered_set<EntityKind>>
{
  return m_allowedTargets;
}

auto ComputerSlotComponent::damageModifier() const -> std::optional<float>
{
  return m_damageModifier;
}

} // namespace bsgo
