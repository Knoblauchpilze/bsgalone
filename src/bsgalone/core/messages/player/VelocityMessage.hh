

#pragma once

#include "AbstractPlayerMessage.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgalone::core {

class VelocityMessage : public AbstractPlayerMessage
{
  public:
  VelocityMessage(const bsgo::Uuid playerDbId,
                  const bsgo::Uuid systemDbId,
                  const bsgo::Uuid shipDbId,
                  const Eigen::Vector3f &acceleration);
  ~VelocityMessage() override = default;

  auto getShipDbId() const -> bsgo::Uuid;
  auto getAcceleration() const -> Eigen::Vector3f;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  /// @brief - Tries to read a velocity message from the input stream. If
  /// it succeeds, a valid velocity message will be returned, otherwise
  /// an empty optional.
  /// The stream will **not be reset** in case the reading fails.
  /// @param in - the stream to read from
  /// @return - the deserialized message or an empty optional if reading
  /// the message's data fails
  static auto readFromStream(std::istream &in) -> std::optional<IMessagePtr>;

  private:
  bsgo::Uuid m_shipDbId{};
  Eigen::Vector3f m_acceleration{Eigen::Vector3f::Zero()};

  /// @brief - Private constructor to help with deserialization. This will
  /// initialize an empty message to be used to deserialize properties into
  /// it.
  VelocityMessage();
};

} // namespace bsgalone::core
