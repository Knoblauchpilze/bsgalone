
#pragma once

#include "ForPublishingPlayerMessage.hh"
#include "IMessageQueue.hh"

namespace bsgalone::core {

class PlayerMessagePublisher : public ForPublishingPlayerMessage
{
  public:
  PlayerMessagePublisher(IMessageQueueShPtr outputMessageQueue);
  ~PlayerMessagePublisher() override = default;

  void publishSystemList(const Uuid playerDbId, const std::vector<System> &systems) override;

  private:
  IMessageQueueShPtr m_outputMessageQueue{};
};

} // namespace bsgalone::core
