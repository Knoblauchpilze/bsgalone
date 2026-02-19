

#pragma once

#include "AbstractPlayerMessage.hh"
#include "DockTransition.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class DockMessage : public AbstractPlayerMessage
{
  public:
  DockMessage(const bsgo::Uuid playerDbId,
              const bsgo::Uuid systemDbId,
              const bsgo::Uuid shipDbId,
              const DockTransition transition);
  ~DockMessage() override = default;

  auto getShipDbId() const -> bsgo::Uuid;
  auto getTransition() const -> DockTransition;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  /// @brief - Tries to read a dock message from the input stream. If
  /// it succeeds, a valid dock message will be returned, otherwise
  /// an empty optional.
  /// The stream will **not be reset** in case the reading fails.
  /// @param in - the stream to read from
  /// @return - the deserialized message or an empty optional if reading
  /// the message's data fails
  static auto readFromStream(std::istream &in) -> std::optional<IMessagePtr>;

  private:
  bsgo::Uuid m_shipDbId{};
  DockTransition m_transition{DockTransition::UNDOCK};

  DockMessage();
};

} // namespace bsgalone::core
