
#include "ComputerSlotComponent.hh"

namespace bsgo {

ComputerSlotComponent::ComputerSlotComponent(const PlayerComputerData &computer)
  : SlotComponent(ComponentType::COMPUTER_SLOT,
                  SlotComponentData{.dbId       = computer.dbId,
                                    .offensive  = computer.offensive,
                                    .powerCost  = computer.powerCost,
                                    .range      = computer.range,
                                    .reloadTime = computer.reloadTime})
  , m_duration(computer.duration)
  , m_allowedTargets(computer.allowedTargets)
  , m_damageModifier(computer.damageModifier)
{}

auto ComputerSlotComponent::duration() const -> std::optional<chrono::TickDuration>
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
