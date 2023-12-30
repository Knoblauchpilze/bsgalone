
#pragma once

#include "AbstractMessageConsumer.hh"

namespace bsgo {

class StatusMessageConsumer : public AbstractMessageConsumer
{
  public:
  StatusMessageConsumer();
  ~StatusMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;
};

} // namespace bsgo
