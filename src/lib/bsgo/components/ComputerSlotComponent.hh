
#pragma once

#include "ComputerRepository.hh"
#include "SlotComponent.hh"

namespace bsgo {

class ComputerSlotComponent : public SlotComponent
{
  public:
  ComputerSlotComponent(const Computer &computer);
  ~ComputerSlotComponent() override = default;

  private:
};

using ComputerSlotComponentShPtr = std::shared_ptr<ComputerSlotComponent>;

} // namespace bsgo
