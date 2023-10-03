
#pragma once

#include "Entity.hh"
#include "IBoundingBox.hh"
#include "TransformSystem.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <unordered_map>

namespace bsgo {

class Coordinator : public utils::CoreObject
{
  public:
  Coordinator();
  virtual ~Coordinator() = default;

  auto createEntity(const EntityKind &kind) -> Uuid;
  auto createEntityWithTransform(const EntityKind &kind, IBoundingBoxPtr bbox) -> Uuid;
  auto getEntity(const Uuid &ent) const -> Entity;

  auto getTransformSystem() -> TransformSystem &;
  auto getTransformSystem() const -> const TransformSystem &;

  private:
  std::unordered_map<Uuid, EntityKind> m_entities{};
  TransformSystem m_transformSystem{};
};

} // namespace bsgo
