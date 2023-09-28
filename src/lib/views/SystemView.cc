
#include "SystemView.hh"

namespace bsgo {

SystemView::SystemView()
  : utils::CoreObject("system")
{
  setService("view");
}

auto SystemView::getShipPosition(const Uuid & /*uuid*/) const -> Eigen::Vector3d
{
  return {};
}

} // namespace bsgo
