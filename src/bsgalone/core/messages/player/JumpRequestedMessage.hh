
#pragma once

#include "AbstractPlayerCommand.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgalone::core {

class JumpRequestedMessage : public AbstractPlayerCommand
{
  public:
  JumpRequestedMessage(const Uuid playerDbId,
                       const Uuid sourceSystemDbId,
                       const Uuid shipDbId,
                       const Uuid destinationSystemDbId);
  ~JumpRequestedMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getDestinationSystem() const -> Uuid;

  auto clone() const -> IMessagePtr override;

  /// @brief - Tries to read a jump requested message from the input stream.
  /// If it succeeds, a valid jump message will be returned, otherwise an
  /// empty optional.
  /// The stream will **not be reset** in case the reading fails.
  /// @param in - the stream to read from
  /// @return - the deserialized message or an empty optional if reading
  /// the message's data fails
  static auto readFromStream(std::istream &in) -> std::optional<IMessagePtr>;

  private:
  Uuid m_shipDbId{};
  Uuid m_destinationSystemDbId{};

  JumpRequestedMessage();

  friend auto operator<<(std::ostream &out, const JumpRequestedMessage &message) -> std::ostream &;
};

auto operator<<(std::ostream &out, const JumpRequestedMessage &message) -> std::ostream &;

} // namespace bsgalone::core
