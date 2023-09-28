
#pragma once

#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <eigen3/Eigen/Eigen>
#include <memory>

namespace bsgo {

class SystemView : public utils::CoreObject
{
  public:
  SystemView();

  auto getShipPosition(const Uuid &uuid) const -> Eigen::Vector3d;
};

using SystemViewShPtr = std::shared_ptr<SystemView>;

} // namespace bsgo
