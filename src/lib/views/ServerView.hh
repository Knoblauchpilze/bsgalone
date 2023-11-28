
#pragma once

#include "Coordinator.hh"
#include "IView.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>

namespace bsgo {

class ServerView : public IView
{
  public:
  ServerView(const CoordinatorShPtr &coordinator, const Repositories &repositories);
  ~ServerView() override = default;

  auto getAllSystems() const -> std::vector<System>;

  struct Bounds
  {
    Eigen::Vector3f min{};
    Eigen::Vector3f max{};
  };
  auto getMapBounds() const -> Bounds;
};

using ServerViewShPtr = std::shared_ptr<ServerView>;

} // namespace bsgo
