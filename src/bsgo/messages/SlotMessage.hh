

#pragma once

#include "AbstractMessage.hh"
#include "Slot.hh"
#include "Uuid.hh"

namespace bsgo {

enum class SlotState
{
  ACTIVATED,
  FIRED
};

class SlotMessage : public AbstractMessage
{
  public:
  SlotMessage(const Uuid &shipEntityId,
              const int slotIndex,
              const Slot &slotType,
              const SlotState &state);
  ~SlotMessage() override = default;

  auto getShipEntityId() const -> Uuid;
  auto getSlotIndex() const -> int;
  auto getSlotType() const -> Slot;
  auto getSlotState() const -> SlotState;

  private:
  Uuid m_shipEntityId{};
  int m_slotIndex{};
  Slot m_slotType{};
  SlotState m_state{};
};

} // namespace bsgo
