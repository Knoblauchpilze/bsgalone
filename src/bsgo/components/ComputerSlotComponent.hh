
#pragma once

#include "EntityKind.hh"
#include "PlayerComputerData.hh"
#include "SlotComponent.hh"
#include <optional>
#include <unordered_set>

namespace bsgo {

class ComputerSlotComponent : public SlotComponent
{
  public:
  ComputerSlotComponent(const PlayerComputerData &computer);
  ~ComputerSlotComponent() override = default;

  auto duration() const -> std::optional<TickDuration>;
  auto allowedTargets() const -> std::optional<std::unordered_set<EntityKind>>;
  auto damageModifier() const -> std::optional<float>;

  private:
  std::optional<TickDuration> m_duration;
  std::optional<std::unordered_set<EntityKind>> m_allowedTargets;
  std::optional<float> m_damageModifier;
};

using ComputerSlotComponentShPtr = std::shared_ptr<ComputerSlotComponent>;

} // namespace bsgo
