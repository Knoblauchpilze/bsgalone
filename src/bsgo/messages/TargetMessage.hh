

#pragma once

#include "EntityKind.hh"
#include "TargetData.hh"
#include "Uuid.hh"
#include "ValidatableMessage.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class TargetMessage : public ValidatableMessage
{
  public:
  TargetMessage();
  TargetMessage(TargetData data, const Eigen::Vector3f &position);
  ~TargetMessage() override = default;

  auto getSourceDbId() const -> Uuid;
  auto getSourceKind() const -> EntityKind;
  auto tryGetTargetDbId() const -> std::optional<Uuid>;
  auto tryGetTargetKind() const -> std::optional<EntityKind>;

  auto tryGetSystemDbId() const -> std::optional<Uuid>;
  void setSystemDbId(const Uuid systemDbId);

  auto getPosition() const -> Eigen::Vector3f;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  TargetData m_data{};
  std::optional<Uuid> m_systemDbId{};
  Eigen::Vector3f m_position{Eigen::Vector3f::Zero()};
};

} // namespace bsgo
