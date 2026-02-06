
#pragma once

#include "Item.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class PurchaseMessage : public NetworkMessage
{
  public:
  PurchaseMessage();
  PurchaseMessage(const Uuid playerDbId, const Item &itemType, const Uuid itemDbId);
  ~PurchaseMessage() override = default;

  auto getPlayerDbId() const -> Uuid;
  auto getItemType() const -> Item;
  auto getItemDbId() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_playerDbId{};
  Item m_itemType{};
  Uuid m_itemDbId{};
};

} // namespace bsgo
