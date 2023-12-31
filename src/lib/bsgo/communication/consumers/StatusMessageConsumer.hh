
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "Services.hh"

namespace bsgo {

class StatusMessageConsumer : public AbstractMessageConsumer
{
  public:
  StatusMessageConsumer(const Services &services, IMessageQueue *const messageQueue);
  ~StatusMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  JumpServiceShPtr m_jumpService{};
  IMessageQueue *const m_messageQueue{};
};

} // namespace bsgo
