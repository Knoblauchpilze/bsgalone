
#pragma once

#include "AbstractView.hh"
#include "GameSession.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <vector>

namespace pge {

class ServerView : public AbstractView
{
  public:
  ServerView(GameSessionShPtr gameSession, const bsgo::Repositories &repositories);
  ~ServerView() override = default;

  bool isReady() const noexcept override;

  auto getPlayerSystem() const -> bsgo::Uuid;
  auto getPlayerSystemName() const -> std::string;
  auto getAllSystems() const -> std::vector<bsgo::System>;

  struct Bounds
  {
    Eigen::Vector3f min{};
    Eigen::Vector3f max{};
  };
  auto getMapBounds() const -> Bounds;

  private:
  GameSessionShPtr m_gameSession{};
  bsgo::Repositories m_repositories{};

  void checkSystemDbIdExists() const;
};

using ServerViewShPtr = std::shared_ptr<ServerView>;

} // namespace pge
