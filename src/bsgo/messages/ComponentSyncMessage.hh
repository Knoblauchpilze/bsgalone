

#pragma once

#include "EntityKind.hh"
#include "NetworkMessage.hh"
#include "Status.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <optional>

namespace bsgo {

class ComponentSyncMessage : public NetworkMessage
{
  public:
  ComponentSyncMessage();
  ComponentSyncMessage(const Uuid entityDbId, const EntityKind entityKind);
  ~ComponentSyncMessage() override = default;

  auto getEntityDbId() const -> Uuid;
  auto getEntityKind() const -> EntityKind;

  void setSystemDbId(const Uuid systemDbId);
  auto tryGetSystemDbId() const -> std::optional<Uuid>;
  auto getSystemDbId() const -> Uuid;

  void setStatus(const Status status);
  auto tryGetStatus() const -> std::optional<Status>;

  void setPosition(const Eigen::Vector3f &position);
  auto tryGetPosition() const -> std::optional<Eigen::Vector3f>;

  void setSpeed(const Eigen::Vector3f &speed);
  auto tryGetSpeed() const -> std::optional<Eigen::Vector3f>;
  void setAcceleration(const Eigen::Vector3f &acceleration);
  auto tryGetAcceleration() const -> std::optional<Eigen::Vector3f>;

  void setHealth(const float health);
  auto tryGetHealth() const -> std::optional<float>;

  void setPower(const float power);
  auto tryGetPower() const -> std::optional<float>;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_entityDbId{};
  EntityKind m_entityKind{};

  std::optional<Uuid> m_systemDbId{};
  std::optional<Status> m_status{};
  std::optional<Eigen::Vector3f> m_position{};
  std::optional<Eigen::Vector3f> m_speed{};
  std::optional<Eigen::Vector3f> m_acceleration{};
  std::optional<float> m_health{};
  std::optional<float> m_power{};
};

} // namespace bsgo
