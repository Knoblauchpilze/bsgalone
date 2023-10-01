
#pragma once

#include "Coordinator.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <unordered_set>

namespace bsgo {

class SystemView : public utils::CoreObject
{
  public:
  SystemView();

  auto getShipPosition(const Uuid &uuid) const -> Eigen::Vector3f;
  auto getAsteroidPositions() const -> std::vector<Eigen::Vector3f>;

  private:
  Coordinator m_coordinator{};

  std::unordered_set<Uuid> m_ships{};
  std::unordered_set<Uuid> m_asteroids{};

  void init();
};

using SystemViewShPtr = std::shared_ptr<SystemView>;

} // namespace bsgo
