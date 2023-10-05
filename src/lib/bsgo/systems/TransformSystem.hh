
#pragma once

#include "IBoundingBox.hh"
#include "Transform.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <optional>
#include <unordered_map>

namespace bsgo {

class TransformSystem : public utils::CoreObject
{
  public:
  TransformSystem();

  void addTransform(const Uuid &ent, IBoundingBoxPtr bbox);
  auto getTransform(const Uuid &ent) const -> std::optional<TransformShPtr>;

  auto getEntityAt(const Eigen::Vector3f &pos) const -> std::optional<Uuid>;
  auto getEntitiesWithin(const IBoundingBox &bbox) const -> std::vector<Uuid>;

  private:
  std::unordered_map<Uuid, TransformShPtr> m_transforms{};
};

} // namespace bsgo
