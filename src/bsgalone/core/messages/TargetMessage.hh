

#pragma once

#include "EntityKind.hh"
#include "NetworkMessage.hh"
#include "Target.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>

namespace bsgalone::core {

class TargetMessage : public NetworkMessage
{
  public:
  TargetMessage();
  TargetMessage(Target data, const Eigen::Vector3f &position);
  ~TargetMessage() override = default;

  auto getSourceDbId() const -> Uuid;
  auto getSourceKind() const -> EntityKind;
  auto tryGetTargetDbId() const -> std::optional<Uuid>;
  auto tryGetTargetKind() const -> std::optional<EntityKind>;

  auto tryGetSystemDbId() const -> std::optional<Uuid>;
  void setSystemDbId(const Uuid systemDbId);

  auto getPosition() const -> Eigen::Vector3f;

  auto clone() const -> IMessagePtr override;

  private:
  Target m_data{};
  std::optional<Uuid> m_systemDbId{};
  Eigen::Vector3f m_position{Eigen::Vector3f::Zero()};

  friend auto operator<<(std::ostream &out, const TargetMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, TargetMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const TargetMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, TargetMessage &message) -> std::istream &;

} // namespace bsgalone::core
