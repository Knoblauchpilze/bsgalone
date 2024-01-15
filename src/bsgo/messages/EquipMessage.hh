
#pragma once

#include "AbstractMessage.hh"
#include "Item.hh"
#include "Uuid.hh"

namespace bsgo {

enum class EquipType
{
  EQUIP,
  UNEQUIP
};

enum class EquipState
{
  REQUESTED,
  COMPLETED
};

class EquipMessage : public AbstractMessage
{
  public:
  EquipMessage(const EquipType &action,
               const Uuid &shipDbId,
               const Item &type,
               const Uuid &itemDbId,
               const EquipState &state);
  ~EquipMessage() override = default;

  auto getAction() const -> EquipType;
  auto getShipDbId() const -> Uuid;
  auto getItemType() const -> Item;
  auto getItemDbId() const -> Uuid;
  auto getEquipState() const -> EquipState;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  auto deserialize(std::istream &in) -> std::istream & override;

  private:
  EquipType m_action{};
  Uuid m_shipDbId{};
  Item m_type{};
  Uuid m_itemDbId{};
  EquipState m_state{};
};

} // namespace bsgo
