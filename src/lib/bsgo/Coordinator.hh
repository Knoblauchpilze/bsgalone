
#pragma once

#include "Entity.hh"
#include "IBoundingBox.hh"
#include "TransformSystem.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <unordered_set>

namespace bsgo {

class Coordinator : public utils::CoreObject
{
  public:
  Coordinator();
  virtual ~Coordinator() = default;

  auto createEntity() -> Uuid;
  auto createEntityWithTransform(IBoundingBoxPtr bbox) -> Uuid;
  auto getEntity(const Uuid &ent) const -> Entity;

  auto getTransformSystem() -> TransformSystem &;

  private:
  std::unordered_set<Uuid> m_entities{};
  TransformSystem m_transformSystem{};
};

} // namespace bsgo
