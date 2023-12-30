
#pragma once

#include "AbstractMessageListener.hh"

namespace bsgo {

class StatusMessageConsumer : public AbstractMessageListener
{
  public:
  StatusMessageConsumer();
  ~StatusMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;
};

} // namespace bsgo
