

#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class VelocityMessage : public NetworkMessage
{
  public:
  VelocityMessage();
  VelocityMessage(const Uuid &shipEntityId, const Eigen::Vector3f &acceleration);
  ~VelocityMessage() override = default;

  auto getShipEntityId() const -> Uuid;
  auto getAcceleration() const -> Eigen::Vector3f;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  private:
  Uuid m_shipEntityId{};
  Eigen::Vector3f m_acceleration{Eigen::Vector3f::Zero()};
};

} // namespace bsgo
