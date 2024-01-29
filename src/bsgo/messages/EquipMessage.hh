
#pragma once

#include "Item.hh"
#include "Uuid.hh"
#include "ValidatableMessage.hh"

namespace bsgo {

enum class EquipType
{
  EQUIP,
  UNEQUIP
};

class EquipMessage : public ValidatableMessage
{
  public:
  EquipMessage();
  EquipMessage(const EquipType &action,
               const Uuid shipDbId,
               const Item &itemType,
               const Uuid itemDbId);
  ~EquipMessage() override = default;

  auto getAction() const -> EquipType;
  auto getShipDbId() const -> Uuid;
  auto getItemType() const -> Item;
  auto getItemDbId() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  EquipType m_action{};
  Uuid m_shipDbId{};
  Item m_itemType{};
  Uuid m_itemDbId{};
};

} // namespace bsgo
