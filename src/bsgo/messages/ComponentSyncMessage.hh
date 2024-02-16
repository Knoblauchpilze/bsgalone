

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
  ComponentSyncMessage(const Uuid entityDbId, const EntityKind entityKind, const Uuid playerDbId);
  ~ComponentSyncMessage() override = default;

  auto getEntityDbId() const -> Uuid;
  auto getEntityKind() const -> EntityKind;
  auto getPlayerDbId() const -> Uuid;

  void setStatus(const Status status);
  auto tryGetStatus() const -> std::optional<Status>;

  void setPosition(const Eigen::Vector3f &position);
  auto tryGetPosition() const -> std::optional<Eigen::Vector3f>;

  void setAcceleration(const Eigen::Vector3f &acceleration);
  auto tryGetAcceleration() const -> std::optional<Eigen::Vector3f>;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_entityDbId{};
  EntityKind m_entityKind{};
  Uuid m_playerDbId{};

  std::optional<Status> m_status{};
  std::optional<Eigen::Vector3f> m_position{};
  std::optional<Eigen::Vector3f> m_acceleration{};
};

} // namespace bsgo
