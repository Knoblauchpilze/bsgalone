

#pragma once

#include "NetworkMessage.hh"
#include "Slot.hh"
#include "Uuid.hh"

namespace bsgo {

class SlotMessage : public NetworkMessage
{
  public:
  SlotMessage();
  SlotMessage(const Uuid shipDbId, const Uuid slotDbId, const Slot &slotType);
  ~SlotMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getSlotDbId() const -> Uuid;
  auto getSlotType() const -> Slot;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_shipDbId{};
  Uuid m_slotDbId{};
  Slot m_slotType{};
};

} // namespace bsgo
