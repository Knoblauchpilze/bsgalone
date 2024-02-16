

#pragma once

#include "EntityKind.hh"
#include "NetworkMessage.hh"
#include "Status.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgo {

class ComponentSyncMessage : public NetworkMessage
{
  public:
  ComponentSyncMessage();
  ComponentSyncMessage(const Uuid entityDbId, const EntityKind entityKind, const Uuid systemDbId);
  ~ComponentSyncMessage() override = default;

  auto getEntityDbId() const -> Uuid;
  auto getEntityKind() const -> EntityKind;
  auto getSystemDbId() const -> Uuid;

  void setStatus(const Status status);
  auto tryGetStatus() const -> std::optional<Status>;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_entityDbId{};
  EntityKind m_entityKind{};
  Uuid m_systemDbId{};

  std::optional<Status> m_status{};
};

} // namespace bsgo
