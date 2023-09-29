
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

auto SystemView::getAsteroidPositions() const -> std::vector<Eigen::Vector3d>
{
  std::vector<Eigen::Vector3d> out;

  out.push_back(Eigen::Vector3d(1.0f, 2.0f, 0.0f));
  out.push_back(Eigen::Vector3d(4.0f, 2.0f, 0.0f));
  out.push_back(Eigen::Vector3d(-2.0f, -6.0f, 0.0f));
  out.push_back(Eigen::Vector3d(-3.0f, -4.0f, 0.0f));

  return out;
}

} // namespace bsgo
