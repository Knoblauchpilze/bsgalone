
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

  void setPlayerDbId(const Uuid &player);
  bool isReady() const noexcept override;

  auto getPlayerSystem() const -> Uuid;
  auto getPlayerSystemName() const -> std::string;
  auto getAllSystems() const -> std::vector<System>;

  struct Bounds
  {
    Eigen::Vector3f min{};
    Eigen::Vector3f max{};
  };
  auto getMapBounds() const -> Bounds;

  private:
  std::optional<Uuid> m_playerDbId{};

  void checkPlayerDbIdExists() const;
};

using ServerViewShPtr = std::shared_ptr<ServerView>;

} // namespace bsgo
