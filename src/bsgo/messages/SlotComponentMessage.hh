
#pragma once

#include "ComponentUpdatedMessage.hh"
#include "SlotComponent.hh"

namespace bsgo {

class SlotComponentMessage : public ComponentUpdatedMessage
{
  public:
  SlotComponentMessage();
  SlotComponentMessage(const Uuid entityId, const int slotIndex, const SlotComponent &component);
  ~SlotComponentMessage() override = default;

  auto getSlotIndex() const -> int;
  auto getElapsedSinceLastFired() const -> std::optional<utils::Duration>;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  private:
  int m_slotIndex{};

  bool m_fireRequest{false};
  FiringState m_firingState{FiringState::READY};
  std::optional<utils::Duration> m_elapsedSinceLastFired{};
};

} // namespace bsgo
