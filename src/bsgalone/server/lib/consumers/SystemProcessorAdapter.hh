
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Uuid.hh"

namespace bsgalone::server {

class SystemProcessorAdapter : public core::AbstractMessageConsumer
{
  public:
  SystemProcessorAdapter(const bsgo::Uuid systemDbId, core::IMessageQueueShPtr systemQueue);
  ~SystemProcessorAdapter() override = default;

  void onMessageReceived(const core::IMessage &message) override;

  private:
  bsgo::Uuid m_systemDbId{};
  core::IMessageQueueShPtr m_systemQueue{};
};

} // namespace bsgalone::server
