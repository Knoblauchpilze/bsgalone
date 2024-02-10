
#pragma once

#include "AbstractMessageConsumer.hh"

namespace bsgo {

class ShipDeletedMessageConsumer : public AbstractMessageConsumer
{
  public:
  ShipDeletedMessageConsumer();
  ~ShipDeletedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
};

} // namespace bsgo
