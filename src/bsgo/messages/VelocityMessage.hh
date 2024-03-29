

#pragma once

#include "Uuid.hh"
#include "ValidatableMessage.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class VelocityMessage : public ValidatableMessage
{
  public:
  VelocityMessage();
  VelocityMessage(const Uuid shipDbId, const Eigen::Vector3f &acceleration);
  ~VelocityMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getAcceleration() const -> Eigen::Vector3f;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_shipDbId{};
  Eigen::Vector3f m_acceleration{Eigen::Vector3f::Zero()};
};

} // namespace bsgo
