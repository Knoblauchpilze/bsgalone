
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "ScannedMessage.hh"
#include "Services.hh"

namespace bsgo {

class ScannedMessageConsumer : public AbstractMessageConsumer
{
  public:
  ScannedMessageConsumer(IMessageQueue *const messageQueue);
  ~ScannedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
  IMessageQueue *const m_messageQueue{};

  void handleScanOperation(const ScannedMessage &message) const;
};

} // namespace bsgo
