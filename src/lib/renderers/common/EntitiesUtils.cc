
#include "EntitiesUtils.hh"
#include "AxisAlignedBoundingBox.hh"

namespace bsgo {

constexpr auto DEFAULT_Z = 0.0f;

auto getEntitiesWithinViewport(const SystemView &system,
                               const pge::CoordinateFrame &frame,
                               const EntityKind &filter) -> std::vector<Entity>
{
  const auto vpPos = frame.tilesViewport().center();
  const Eigen::Vector3f p(vpPos.x, vpPos.y, DEFAULT_Z);
  const auto vpDims = frame.tilesViewport().dims();
  const Eigen::Vector3f dims(vpDims.x, vpDims.y, DEFAULT_Z);
  bsgo::AxisAlignedBoundingBox bbox(p, dims);
  return system.getEntitiesWithin(bbox, filter);
}

} // namespace bsgo
