

#pragma once

#include "EntityKind.hh"
#include "Uuid.hh"
#include "ValidatableMessage.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class TargetMessage : public ValidatableMessage
{
  public:
  TargetMessage();
  TargetMessage(const Uuid shipDbId, const Eigen::Vector3f &position);
  TargetMessage(const Uuid shipDbId,
                const Eigen::Vector3f &position,
                const std::optional<EntityKind> &targetKind,
                const std::optional<Uuid> &targetDbId);
  ~TargetMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getPosition() const -> Eigen::Vector3f;
  auto getTargetKind() const -> std::optional<EntityKind>;
  auto getTargetDbId() const -> std::optional<Uuid>;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_shipDbId{};
  Eigen::Vector3f m_position{Eigen::Vector3f::Zero()};
  std::optional<EntityKind> m_targetKind{};
  std::optional<Uuid> m_targetDbId{};
};

} // namespace bsgo
