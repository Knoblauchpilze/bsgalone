
#pragma once

#include "AbstractRepository.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>

namespace bsgo {

struct AiBehavior
{
  int targetIndex{};
  std::vector<Eigen::Vector3f> targets{};
};

class AiBehaviorRepository : public AbstractRepository
{
  public:
  AiBehaviorRepository(const DbConnectionShPtr &connection);
  ~AiBehaviorRepository() override = default;

  void initialize() override;

  auto findOneByShip(const Uuid shipDbId) const -> AiBehavior;

  private:
  void fetchAiTargets(const Uuid shipDbId, AiBehavior &out) const;
};

using AiBehaviorRepositoryShPtr = std::shared_ptr<AiBehaviorRepository>;

} // namespace bsgo
