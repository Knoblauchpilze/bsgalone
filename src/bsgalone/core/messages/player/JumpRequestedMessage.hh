
#pragma once

#include "AbstractPlayerMessage.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgalone::core {

class JumpRequestedMessage : public AbstractPlayerMessage
{
  public:
  JumpRequestedMessage(const bsgo::Uuid playerDbId,
                       const bsgo::Uuid sourceSystemDbId,
                       const bsgo::Uuid shipDbId,
                       const bsgo::Uuid destinationSystemDbId);
  ~JumpRequestedMessage() override = default;

  auto getShipDbId() const -> bsgo::Uuid;
  auto getDestinationSystem() const -> bsgo::Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  /// @brief - Tries to read a jump requested message from the input stream.
  /// If it succeeds, a valid dock message will be returned, otherwise an
  /// empty optional.
  /// The stream will **not be reset** in case the reading fails.
  /// @param in - the stream to read from
  /// @return - the deserialized message or an empty optional if reading
  /// the message's data fails
  static auto readFromStream(std::istream &in) -> std::optional<IMessagePtr>;

  private:
  bsgo::Uuid m_shipDbId{};
  bsgo::Uuid m_destinationSystemDbId{};

  JumpRequestedMessage();
};

} // namespace bsgalone::core
