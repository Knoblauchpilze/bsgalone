
#include "ComputerSlotComponent.hh"

namespace bsgo {

ComputerSlotComponent::ComputerSlotComponent(const Computer &computer)
  : SlotComponent("computer",
                  SlotComponentData{.powerCost  = computer.powerCost,
                                    .range      = computer.range,
                                    .reloadTime = computer.reloadTime})
  , m_duration(computer.duration)
  , m_damageModifier(computer.damageModifier)
{}

auto ComputerSlotComponent::duration() const -> std::optional<utils::Duration>
{
  return m_duration;
}

auto ComputerSlotComponent::damageModifier() const -> std::optional<float>
{
  return m_damageModifier;
}

} // namespace bsgo
