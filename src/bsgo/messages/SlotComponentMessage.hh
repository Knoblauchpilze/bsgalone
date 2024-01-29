
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

  auto clone() const -> IMessagePtr override;

  private:
  int m_slotIndex{};
  std::optional<utils::Duration> m_elapsedSinceLastFired{};
};

} // namespace bsgo
