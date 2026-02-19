
#pragma once

#include "AbstractPlayerMessage.hh"
#include "Item.hh"
#include "Uuid.hh"

namespace bsgalone::core {

enum class EquipType
{
  EQUIP,
  UNEQUIP
};

class EquipMessage : public AbstractPlayerMessage
{
  public:
  EquipMessage(const bsgo::Uuid playerDbId,
               const bsgo::Uuid systemDbId,
               const EquipType &action,
               const bsgo::Uuid shipDbId,
               const Item &itemType,
               const bsgo::Uuid itemDbId);
  ~EquipMessage() override = default;

  auto getAction() const -> EquipType;
  auto getShipDbId() const -> bsgo::Uuid;
  auto getItemType() const -> Item;
  auto getItemDbId() const -> bsgo::Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  /// @brief - Tries to read a equip message from the input stream. If
  /// it succeeds, a valid equip message will be returned, otherwise
  /// an empty optional.
  /// The stream will **not be reset** in case the reading fails.
  /// @param in - the stream to read from
  /// @return - the deserialized message or an empty optional if reading
  /// the message's data fails
  static auto readFromStream(std::istream &in) -> std::optional<IMessagePtr>;

  private:
  EquipType m_action{};
  bsgo::Uuid m_shipDbId{};
  Item m_itemType{};
  bsgo::Uuid m_itemDbId{};

  EquipMessage();
};

} // namespace bsgalone::core
