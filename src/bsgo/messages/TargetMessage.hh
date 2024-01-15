

#pragma once

#include "AbstractMessage.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class TargetMessage : public AbstractMessage
{
  public:
  TargetMessage(const Uuid &shipEntityId, const Eigen::Vector3f &position);
  ~TargetMessage() override = default;

  auto getShipEntityId() const -> Uuid;
  auto getPosition() const -> Eigen::Vector3f;

  auto operator<<(std::ostream &out) const -> std::ostream & override;
  auto operator>>(std::istream &in) -> std::istream & override;

  private:
  Uuid m_shipEntityId{};
  Eigen::Vector3f m_position{};
};

} // namespace bsgo
