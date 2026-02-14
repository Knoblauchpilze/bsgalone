
#pragma once

#include "AbstractView.hh"
#include "GameSession.hh"
#include "SystemData.hh"
#include "Uuid.hh"
#include <eigen3/Eigen/Eigen>
#include <memory>
#include <vector>

namespace pge {

class ServerView : public AbstractView
{
  public:
  ServerView(GameSessionShPtr gameSession);
  ~ServerView() override = default;

  bool isReady() const noexcept override;
  void reset() override;

  auto getPlayerSystem() const -> bsgo::Uuid;
  auto getPlayerSystemName() const -> std::string;
  auto getAllSystems() const -> std::vector<bsgo::SystemData>;

  struct Bounds
  {
    Eigen::Vector3f min{};
    Eigen::Vector3f max{};
  };
  auto getMapBounds() const -> Bounds;

  protected:
  void handleMessageInternal(const bsgalone::core::IMessage &message) override;

  private:
  GameSessionShPtr m_gameSession{};
  std::vector<bsgo::SystemData> m_systems{};
};

using ServerViewShPtr = std::shared_ptr<ServerView>;

} // namespace pge
