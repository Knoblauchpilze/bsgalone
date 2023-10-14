
#pragma once

#include "Coordinator.hh"
#include "IView.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>

namespace bsgo {

class SystemView : public IView
{
  public:
  SystemView(const CoordinatorShPtr &coordinator);
  ~SystemView() override = default;

  auto getEntityAt(const Eigen::Vector3f &pos) const -> std::optional<Entity>;
  auto getAsteroidsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>;
  auto getOutpostsWithin(const IBoundingBox &bbox) const -> std::vector<Entity>;

  private:
  CoordinatorShPtr m_coordinator{};
};

using SystemViewShPtr = std::shared_ptr<SystemView>;

} // namespace bsgo
