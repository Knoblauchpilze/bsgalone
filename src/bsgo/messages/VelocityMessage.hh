

#pragma once

#include "AbstractMessage.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class VelocityMessage : public AbstractMessage
{
  public:
  VelocityMessage(const Uuid &shipEntityId, const Eigen::Vector3f &acceleration);
  ~VelocityMessage() override = default;

  auto getShipEntityId() const -> Uuid;
  auto getAcceleration() const -> Eigen::Vector3f;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  auto deserialize(std::istream &in) -> std::istream & override;

  private:
  Uuid m_shipEntityId{};
  Eigen::Vector3f m_acceleration{};
};

} // namespace bsgo
