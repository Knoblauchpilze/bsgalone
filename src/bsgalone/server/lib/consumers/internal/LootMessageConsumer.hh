
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "SystemService.hh"

namespace bsgalone::server {

class LootMessageConsumer : public core::AbstractMessageConsumer
{
  public:
  LootMessageConsumer(SystemServiceShPtr systemService,
                      core::IMessageQueue *const outputMessageQueue);
  ~LootMessageConsumer() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  core::IMessageQueue *const m_outputMessageQueue{};
};

} // namespace bsgalone::server
