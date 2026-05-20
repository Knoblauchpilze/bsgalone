
#pragma once

#include "ForExecutingUndock.hh"
#include "ForManagingPlayer.hh"
#include "ForManagingSystem.hh"
#include "ForPublishingEvent.hh"
#include "SystemsManager.hh"

namespace bsgalone::server {

class UndockUseCase : public ForExecutingUndock
{
  public:
  UndockUseCase(ForManagingPlayerShPtr playerRepo,
                ForManagingSystemShPtr systemRepo,
                SystemsManagerShPtr systemsManager,
                ForPublishingEventShPtr eventPublisher);
  ~UndockUseCase() override = default;

  void performUndock(const UndockData &data) override;

  private:
  ForManagingPlayerShPtr m_playerRepo{};
  ForManagingSystemShPtr m_systemRepo{};
  SystemsManagerShPtr m_systemsManager{};
  ForPublishingEventShPtr m_eventPublisher{};
};

} // namespace bsgalone::server
