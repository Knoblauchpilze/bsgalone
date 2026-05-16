
#pragma once

#include "ForExecutingUndock.hh"
#include "ForManagingPlayer.hh"
#include "ForPublishingEvent.hh"

namespace bsgalone::server {

class UndockUseCase : public ForExecutingUndock
{
  public:
  UndockUseCase(ForManagingPlayerShPtr playerRepo, ForPublishingEventShPtr eventPublisher);
  ~UndockUseCase() override = default;

  void performUndock(const UndockData &data) override;

  private:
  ForManagingPlayerShPtr m_playerRepo{};
  ForPublishingEventShPtr m_eventPublisher{};
};

} // namespace bsgalone::server
