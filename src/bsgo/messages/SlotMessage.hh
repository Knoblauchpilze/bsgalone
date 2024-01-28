

#pragma once

#include "NetworkMessage.hh"
#include "Slot.hh"
#include "Uuid.hh"

namespace bsgo {

class SlotMessage : public NetworkMessage
{
  public:
  SlotMessage();
  SlotMessage(const Uuid &shipEntityId, const int slotIndex, const Slot &slotType);
  ~SlotMessage() override = default;

  auto getShipEntityId() const -> Uuid;
  auto getSlotIndex() const -> int;
  auto getSlotType() const -> Slot;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  private:
  Uuid m_shipEntityId{};
  int m_slotIndex{};
  Slot m_slotType{};
};

} // namespace bsgo
