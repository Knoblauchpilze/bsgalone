
#pragma once

#include "Coordinator.hh"
#include "IView.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <optional>
#include <unordered_set>

namespace bsgo {

class SystemView : public utils::CoreObject, public IView
{
  public:
  SystemView(const CoordinatorShPtr &coordinator, const Repositories &repositories);
  ~SystemView() override = default;

  void update(const float elapsedSeconds) override;

  auto getEntity(const Uuid &ent) const -> Entity;
  auto getEntityAt(const Eigen::Vector3f &pos, const std::optional<EntityKind> &filter = {}) const
    -> std::optional<Entity>;
  auto getEntitiesWithin(const IBoundingBox &bbox,
                         const std::optional<EntityKind> &filter = {}) const -> std::vector<Entity>;

  private:
  CoordinatorShPtr m_coordinator{};

  std::unordered_set<Uuid> m_ships{};
  std::unordered_set<Uuid> m_asteroids{};

  void init(const Repositories &repositories);
  bool isValidForFilter(const Uuid &ent, const EntityKind &kind) const;
};

using SystemViewShPtr = std::shared_ptr<SystemView>;

} // namespace bsgo
