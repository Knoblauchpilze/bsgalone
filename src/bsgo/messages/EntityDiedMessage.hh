

#pragma once

#include "EntityKind.hh"
#include "Uuid.hh"
#include "ValidatableMessage.hh"

namespace bsgo {

class EntityDiedMessage : public ValidatableMessage
{
  public:
  EntityDiedMessage();
  EntityDiedMessage(const Uuid entityDbId, const EntityKind entityKind);
  ~EntityDiedMessage() override = default;

  auto getEntityDbId() const -> Uuid;
  auto getEntityKind() const -> EntityKind;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_entityDbId{};
  EntityKind m_entityKind{};
};

} // namespace bsgo
