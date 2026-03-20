

#pragma once

#include "AbstractPlayerCommand.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class ShipSelectedMessage : public AbstractPlayerCommand
{
  public:
  ShipSelectedMessage(const Uuid playerDbId, const Uuid systemDbId, const Uuid shipDbId);
  ~ShipSelectedMessage() override = default;

  auto getShipDbId() const -> Uuid;

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
  Uuid m_shipDbId{};

  ShipSelectedMessage();

  friend auto operator<<(std::ostream &out, const ShipSelectedMessage &message) -> std::ostream &;
};

auto operator<<(std::ostream &out, const ShipSelectedMessage &message) -> std::ostream &;

} // namespace bsgalone::core
