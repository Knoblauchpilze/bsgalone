
#pragma once

#include "AbstractView.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <vector>

namespace pge {

class ServerView : public AbstractView
{
  public:
  ServerView(const bsgo::Repositories &repositories);
  ~ServerView() override = default;

  void setPlayerDbId(const bsgo::Uuid player);
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
  bsgo::Repositories m_repositories{};
  std::optional<bsgo::Uuid> m_playerDbId{};

  void checkPlayerDbIdExists() const;
};

using ServerViewShPtr = std::shared_ptr<ServerView>;

} // namespace pge
