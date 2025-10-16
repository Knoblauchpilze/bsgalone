
#pragma once

#include "ComponentUpdatedMessage.hh"
#include "SlotComponent.hh"

namespace bsgo {

class SlotComponentMessage : public ComponentUpdatedMessage
{
  public:
  SlotComponentMessage();
  SlotComponentMessage(const Uuid playerDbId,
                       const Uuid shipDbId,
                       const int slotDbId,
                       const std::optional<chrono::TickDuration> &elapsedSinceLastFired);
  ~SlotComponentMessage() override = default;

  auto getPlayerDbId() const -> Uuid;
  auto getSlotDbId() const -> Uuid;
  auto getElapsedSinceLastFired() const -> std::optional<chrono::TickDuration>;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_playerDbId{};
  Uuid m_slotDbId{};
  std::optional<chrono::TickDuration> m_elapsedSinceLastFired{};
};

} // namespace bsgo
