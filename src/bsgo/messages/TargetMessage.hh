

#pragma once

#include "EntityKind.hh"
#include "NetworkMessage.hh"
#include "Target.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgo {

class TargetMessage : public bsgalone::core::NetworkMessage
{
  public:
  TargetMessage();
  TargetMessage(bsgalone::core::Target data, const Eigen::Vector3f &position);
  ~TargetMessage() override = default;

  auto getSourceDbId() const -> Uuid;
  auto getSourceKind() const -> bsgalone::core::EntityKind;
  auto tryGetTargetDbId() const -> std::optional<Uuid>;
  auto tryGetTargetKind() const -> std::optional<bsgalone::core::EntityKind>;

  auto tryGetSystemDbId() const -> std::optional<Uuid>;
  void setSystemDbId(const Uuid systemDbId);

  auto getPosition() const -> Eigen::Vector3f;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> bsgalone::core::IMessagePtr override;

  private:
  bsgalone::core::Target m_data{};
  std::optional<Uuid> m_systemDbId{};
  Eigen::Vector3f m_position{Eigen::Vector3f::Zero()};
};

} // namespace bsgo
