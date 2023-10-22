
#pragma once

#include "ComputerRepository.hh"
#include "SlotComponent.hh"
#include <core_utils/TimeUtils.hh>
#include <optional>

namespace bsgo {

class ComputerSlotComponent : public SlotComponent
{
  public:
  ComputerSlotComponent(const Computer &computer);
  ~ComputerSlotComponent() override = default;

  auto duration() const -> std::optional<utils::Duration>;
  auto damageModifier() const -> std::optional<float>;

  private:
  std::optional<utils::Duration> m_duration{};
  std::optional<float> m_damageModifier{};
};

using ComputerSlotComponentShPtr = std::shared_ptr<ComputerSlotComponent>;

} // namespace bsgo
