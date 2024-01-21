
#pragma once

#include "Item.hh"
#include "Uuid.hh"
#include "ValidatableMessage.hh"

namespace bsgo {

class PurchaseMessage : public ValidatableMessage
{
  public:
  PurchaseMessage();
  PurchaseMessage(const Uuid &playerDbId, const Item &type, const Uuid &itemDbId);
  ~PurchaseMessage() override = default;

  auto getPlayerDbId() const -> Uuid;
  auto getItemType() const -> Item;
  auto getItemDbId() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  private:
  Uuid m_playerDbId{};
  Item m_type{};
  Uuid m_itemDbId{};
};

} // namespace bsgo
