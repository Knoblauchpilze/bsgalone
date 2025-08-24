
#pragma once

#include "AbstractView.hh"
#include "Coordinator.hh"
#include "DatabaseEntityMapper.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>

namespace pge {

class SystemView : public AbstractView
{
  public:
  SystemView(bsgo::CoordinatorShPtr coordinator, const bsgo::DatabaseEntityMapper &entityMapper);
  ~SystemView() override = default;

  void reset() override;

  auto getAsteroidsWithin(const bsgo::IBoundingBox &bbox) const -> std::vector<bsgo::Entity>;
  auto getOutpostsWithin(const bsgo::IBoundingBox &bbox) const -> std::vector<bsgo::Entity>;
  auto getBulletsWithin(const bsgo::IBoundingBox &bbox) const -> std::vector<bsgo::Entity>;

  auto getAsteroid(const bsgo::Uuid asteroidDbId) const -> bsgo::Entity;

  private:
  bsgo::CoordinatorShPtr m_coordinator{};
  const bsgo::DatabaseEntityMapper &m_entityMapper;
};

using SystemViewShPtr = std::shared_ptr<SystemView>;

} // namespace pge
