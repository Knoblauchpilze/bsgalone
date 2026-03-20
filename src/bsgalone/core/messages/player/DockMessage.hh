

#pragma once

#include "AbstractPlayerCommand.hh"
#include "DockTransition.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class DockMessage : public AbstractPlayerCommand
{
  public:
  DockMessage(const Uuid playerDbId,
              const Uuid systemDbId,
              const Uuid shipDbId,
              const DockTransition transition);
  ~DockMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getTransition() const -> DockTransition;

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
  DockTransition m_transition{DockTransition::UNDOCK};

  DockMessage();

  friend auto operator<<(std::ostream &out, const DockMessage &message) -> std::ostream &;
};

auto operator<<(std::ostream &out, const DockMessage &message) -> std::ostream &;

} // namespace bsgalone::core
