
#pragma once

#include "AbstractPlayerMessage.hh"
#include "Item.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class PurchaseMessage : public AbstractPlayerMessage
{
  public:
  PurchaseMessage(const bsgo::Uuid playerDbId,
                  const bsgo::Uuid systemDbId,
                  const Item &itemType,
                  const bsgo::Uuid itemDbId);
  ~PurchaseMessage() override = default;

  auto getItemType() const -> Item;
  auto getItemDbId() const -> bsgo::Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  /// @brief - Tries to read a purchase message from the input stream. If
  /// it succeeds, a valid purchase message will be returned, otherwise
  /// an empty optional.
  /// The stream will **not be reset** in case the reading fails.
  /// @param in - the stream to read from
  /// @return - the deserialized message or an empty optional if reading
  /// the message's data fails
  static auto readFromStream(std::istream &in) -> std::optional<IMessagePtr>;

  private:
  Item m_itemType{};
  bsgo::Uuid m_itemDbId{};

  PurchaseMessage();
};

} // namespace bsgalone::core
