

#pragma once

#include "AbstractPlayerMessage.hh"
#include "Target.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgalone::core {

class TargetPickedMessage : public AbstractPlayerMessage
{
  public:
  TargetPickedMessage(const bsgo::Uuid playerDbId,
                      const bsgo::Uuid systemDbId,
                      Target data,
                      const Eigen::Vector3f &position);
  ~TargetPickedMessage() override = default;

  auto getTargetData() const -> Target;
  auto getPosition() const -> Eigen::Vector3f;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  /// @brief - Tries to read a target message from the input stream. If
  /// it succeeds, a valid target message will be returned, otherwise
  /// an empty optional.
  /// The stream will **not be reset** in case the reading fails.
  /// @param in - the stream to read from
  /// @return - the deserialized message or an empty optional if reading
  /// the message's data fails
  static auto readFromStream(std::istream &in) -> std::optional<IMessagePtr>;

  private:
  Target m_data{};
  Eigen::Vector3f m_position{Eigen::Vector3f::Zero()};

  TargetPickedMessage();
};

} // namespace bsgalone::core
