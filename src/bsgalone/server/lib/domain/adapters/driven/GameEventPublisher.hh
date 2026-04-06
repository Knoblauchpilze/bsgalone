
#pragma once

#include "ForPublishingEvent.hh"
#include "IGameEventQueue.hh"

namespace bsgalone::server {

class GameEventPublisher : public ForPublishingEvent
{
  public:
  GameEventPublisher(server::IGameEventQueueShPtr queue);
  ~GameEventPublisher() override = default;

  void publishEvent(IGameEventPtr event) override;

  private:
  server::IGameEventQueueShPtr m_queue{};
};

} // namespace bsgalone::server
