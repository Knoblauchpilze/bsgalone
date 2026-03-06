
#include "EntitiesUtils.hh"

namespace bsgalone::client {

constexpr auto DEFAULT_Z = 0.0f;

auto toIBoundingBox(const pge::CoordinateFrame &frame) -> core::AxisAlignedBoundingBox
{
  const auto vpPos = frame.tilesViewport().center();
  const Eigen::Vector3f p(vpPos.x, vpPos.y, DEFAULT_Z);
  const auto vpDims = frame.tilesViewport().dims();
  const Eigen::Vector3f dims(vpDims.x, vpDims.y, DEFAULT_Z);
  return core::AxisAlignedBoundingBox(p, dims);
}

} // namespace bsgalone::client
