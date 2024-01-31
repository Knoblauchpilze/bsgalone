
#pragma once

#include "AbstractView.hh"
#include "Coordinator.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>

namespace bsgo {

class ServerView : public AbstractView
{
  public:
  ServerView(const CoordinatorShPtr &coordinator,
             const Repositories &repositories,
             IMessageQueue *const messageQueue);
  ~ServerView() override = default;

  void setPlayerDbId(const Uuid player);
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
