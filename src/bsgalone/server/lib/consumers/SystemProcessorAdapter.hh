
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Uuid.hh"

namespace bsgalone::server {

class SystemProcessorAdapter : public core::AbstractMessageConsumer
{
  public:
  SystemProcessorAdapter(const core::Uuid systemDbId, core::IMessageQueueShPtr systemQueue);
  ~SystemProcessorAdapter() override = default;

  void onEventReceived(const core::IMessage &message) override;

  private:
  core::Uuid m_systemDbId{};
  core::IMessageQueueShPtr m_systemQueue{};
};

} // namespace bsgalone::server
