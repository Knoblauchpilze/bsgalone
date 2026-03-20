

#pragma once

#include "AbstractPlayerCommand.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgalone::core {

class VelocityMessage : public AbstractPlayerCommand
{
  public:
  VelocityMessage(const Uuid playerDbId,
                  const Uuid systemDbId,
                  const Uuid shipDbId,
                  const Eigen::Vector3f &acceleration);
  ~VelocityMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getAcceleration() const -> Eigen::Vector3f;

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
  Eigen::Vector3f m_acceleration{Eigen::Vector3f::Zero()};

  /// @brief - Private constructor to help with deserialization. This will
  /// initialize an empty message to be used to deserialize properties into
  /// it.
  VelocityMessage();

  friend auto operator<<(std::ostream &out, const VelocityMessage &message) -> std::ostream &;
};

auto operator<<(std::ostream &out, const VelocityMessage &message) -> std::ostream &;

} // namespace bsgalone::core
