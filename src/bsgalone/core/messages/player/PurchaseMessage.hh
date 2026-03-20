
#pragma once

#include "AbstractPlayerCommand.hh"
#include "Item.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class PurchaseMessage : public AbstractPlayerCommand
{
  public:
  PurchaseMessage(const Uuid playerDbId,
                  const Uuid systemDbId,
                  const Item &itemType,
                  const Uuid itemDbId);
  ~PurchaseMessage() override = default;

  auto getItemType() const -> Item;
  auto getItemDbId() const -> Uuid;

  auto clone() const -> IMessagePtr override;

  /// @brief - Tries to read a message from the input stream. If it
  /// succeeds, a valid dock message will be returned, otherwise an
  /// empty optional.
  /// The stream will **not be reset** in case the reading fails.
  /// @param in - the stream to read from
  /// @return - the deserialized message or an empty optional if reading
  /// the message's data fails
  static auto readFromStream(std::istream &in) -> std::optional<IMessagePtr>;

  private:
  Item m_itemType{};
  Uuid m_itemDbId{};

  PurchaseMessage();

  friend auto operator<<(std::ostream &out, const PurchaseMessage &message) -> std::ostream &;
};

auto operator<<(std::ostream &out, const PurchaseMessage &message) -> std::ostream &;

} // namespace bsgalone::core
