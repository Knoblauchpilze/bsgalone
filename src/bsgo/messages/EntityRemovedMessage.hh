

#pragma once

#include "EntityKind.hh"
#include "NetworkMessage.hh"
#include "Uuid.hh"
#include <optional>

namespace bsgo {

class EntityRemovedMessage : public NetworkMessage
{
  public:
  EntityRemovedMessage();
  EntityRemovedMessage(const Uuid entityDbId, const EntityKind entityKind, const bool dead);
  EntityRemovedMessage(const Uuid entityDbId,
                       const EntityKind entityKind,
                       const bool dead,
                       const Uuid systemDbId);
  ~EntityRemovedMessage() override = default;

  auto getEntityDbId() const -> Uuid;
  auto getEntityKind() const -> EntityKind;
  bool isDead() const;
  auto getSystemDbId() const -> Uuid;
  auto tryGetSystemDbId() const -> std::optional<Uuid>;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_entityDbId{};
  EntityKind m_entityKind{};
  bool m_dead{false};
  std::optional<Uuid> m_systemDbId{};
};

} // namespace bsgo
