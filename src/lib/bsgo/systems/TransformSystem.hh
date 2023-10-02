
#pragma once

#include "IBoundingBox.hh"
#include "TransformComponent.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <optional>
#include <unordered_map>

namespace bsgo {

class TransformSystem : public utils::CoreObject
{
  public:
  TransformSystem();

  void addTransformComponent(const Uuid &ent, IBoundingBoxPtr bbox);
  auto getTransformComponent(const Uuid &ent) const -> std::optional<TransformComponent>;

  private:
  std::unordered_map<Uuid, TransformComponent> m_transforms{};
};

} // namespace bsgo
