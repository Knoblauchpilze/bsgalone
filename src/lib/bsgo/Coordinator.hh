
#pragma once

#include "IBoundingBox.hh"
#include "TransformComponent.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <optional>
#include <unordered_set>

namespace bsgo {

class Coordinator : public utils::CoreObject
{
  public:
  Coordinator();
  virtual ~Coordinator() = default;

  auto createEntity() -> Uuid;

  void addTransformComponent(const Uuid &ent, IBoundingBoxPtr bbox);
  auto getTransformComponent(const Uuid &ent) const -> std::optional<TransformComponent>;

  private:
  std::unordered_set<Uuid> m_entities{};
  std::unordered_map<Uuid, TransformComponent> m_transforms{};
};

} // namespace bsgo
