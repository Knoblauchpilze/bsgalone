
#pragma once

#include "AbstractMessage.hh"
#include "Item.hh"
#include "Uuid.hh"

namespace bsgo {

enum class PurchaseState
{
  REQUESTED,
  COMPLETED
};

class PurchaseMessage : public AbstractMessage
{
  public:
  PurchaseMessage(const Uuid &playerDbId,
                  const Item &type,
                  const Uuid &itemDbId,
                  const PurchaseState &state);
  ~PurchaseMessage() override = default;

  auto getPlayerDbId() const -> Uuid;
  auto getItemType() const -> Item;
  auto getItemDbId() const -> Uuid;
  auto getPurchaseState() const -> PurchaseState;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  auto deserialize(std::istream &in) -> std::istream & override;

  private:
  Uuid m_playerDbId{};
  Item m_type{};
  Uuid m_itemDbId{};
  PurchaseState m_state{};
};

} // namespace bsgo
