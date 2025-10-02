
#include "ComputerSlotComponent.hh"

namespace bsgo {

ComputerSlotComponent::ComputerSlotComponent(const PlayerComputerData &computer)
  : SlotComponent(ComponentType::COMPUTER_SLOT,
                  SlotComponentData{.dbId      = computer.dbId,
                                    .offensive = computer.offensive,
                                    .powerCost = computer.powerCost,
                                    .range     = computer.range,
                                    // TODO: We should not convert to milliseconds here
                                    .reloadTime = core::toMilliseconds(computer.reloadTime.count())})
  , m_allowedTargets(computer.allowedTargets)
  , m_damageModifier(computer.damageModifier)
{
  // TODO: We should not convert to milliseconds here.
  if (computer.duration)
  {
    m_duration = core::toMilliseconds(computer.duration->count());
  }
}

auto ComputerSlotComponent::duration() const -> std::optional<core::Duration>
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
