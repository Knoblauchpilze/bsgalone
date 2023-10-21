
#include "ComputerSlotComponent.hh"

namespace bsgo {

ComputerSlotComponent::ComputerSlotComponent(const Computer &computer)
  : SlotComponent("computer",
                  SlotComponentData{.powerCost  = computer.powerCost,
                                    .range      = computer.range,
                                    .reloadTime = computer.reloadTime})
{}

} // namespace bsgo
