
#pragma once

#include "ForPublishingLoginData.hh"
#include "IGameEventListener.hh"

namespace bsgalone::core {

class PlayerLoginEventConsumer : public IGameEventListener
{
  public:
  PlayerLoginEventConsumer(ForPublishingLoginDataPtr useCase);
  ~PlayerLoginEventConsumer() override = default;

  bool isEventRelevant(const GameEventType &type) const override;
  void onEventReceived(const IGameEvent &event) override;

  private:
  ForPublishingLoginDataPtr m_useCase{};
};

} // namespace bsgalone::core
