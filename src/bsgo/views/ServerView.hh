
#pragma once

#include "AbstractView.hh"
#include "Repositories.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <vector>

namespace bsgo {

class ServerView : public AbstractView
{
  public:
  ServerView(const Repositories &repositories);
  ~ServerView() override = default;

  void setPlayerDbId(const std::optional<Uuid> player);
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
  Repositories m_repositories{};
  std::optional<Uuid> m_playerDbId{};

  void checkPlayerDbIdExists() const;
};

using ServerViewShPtr = std::shared_ptr<ServerView>;

} // namespace bsgo
