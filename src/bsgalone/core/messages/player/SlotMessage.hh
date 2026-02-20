

#pragma once

#include "AbstractPlayerMessage.hh"
#include "Slot.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class SlotMessage : public AbstractPlayerMessage
{
  public:
  SlotMessage(const bsgo::Uuid playerDbId,
              const bsgo::Uuid systemDbId,
              const bsgo::Uuid shipDbId,
              const bsgo::Uuid slotDbId,
              const Slot slotType);
  ~SlotMessage() override = default;

  auto getShipDbId() const -> bsgo::Uuid;
  auto getSlotDbId() const -> bsgo::Uuid;
  auto getSlotType() const -> Slot;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  /// @brief - Tries to read a velocity message from the input stream. If
  /// it succeeds, a valid velocity message will be returned, otherwise
  /// an empty optional.
  /// The stream will **not be reset** in case the reading fails.
  /// @param in - the stream to read from
  /// @return - the deserialized message or an empty optional if reading
  /// the message's data fails
  static auto readFromStream(std::istream &in) -> std::optional<IMessagePtr>;

  private:
  bsgo::Uuid m_shipDbId{};
  bsgo::Uuid m_slotDbId{};
  Slot m_slotType{};

  SlotMessage();
};

} // namespace bsgalone::core
