
#pragma once

#include "ComponentUpdatedMessage.hh"
#include "SlotComponent.hh"

namespace bsgalone::core {

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

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_playerDbId{};
  Uuid m_slotDbId{};
  std::optional<chrono::TickDuration> m_elapsedSinceLastFired{};

  friend auto operator<<(std::ostream &out, const SlotComponentMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, SlotComponentMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const SlotComponentMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, SlotComponentMessage &message) -> std::istream &;

} // namespace bsgalone::core
