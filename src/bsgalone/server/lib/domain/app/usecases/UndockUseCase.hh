
#pragma once

#include "ForExecutingUndock.hh"
#include "ForPublishingEvent.hh"

namespace bsgalone::server {

class UndockUseCase : public ForExecutingUndock
{
  public:
  UndockUseCase(ForPublishingEventShPtr eventPublisher);
  ~UndockUseCase() override = default;

  void performUndock(const UndockData &data) override;

  private:
  ForPublishingEventShPtr m_eventPublisher{};
};

} // namespace bsgalone::server
