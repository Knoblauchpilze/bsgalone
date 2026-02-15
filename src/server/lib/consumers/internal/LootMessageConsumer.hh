
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "SystemService.hh"

namespace bsgo {

class LootMessageConsumer : public AbstractMessageConsumer
{
  public:
  LootMessageConsumer(SystemServiceShPtr systemService,
                      bsgalone::core::IMessageQueue *const outputMessageQueue);
  ~LootMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  SystemServiceShPtr m_systemService{};
  bsgalone::core::IMessageQueue *const m_outputMessageQueue{};
};

} // namespace bsgo
