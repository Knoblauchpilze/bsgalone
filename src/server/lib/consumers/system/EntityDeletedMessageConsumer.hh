
#pragma once

#include "AbstractMessageConsumer.hh"

namespace bsgo {

class EntityDeletedMessageConsumer : public AbstractMessageConsumer
{
  public:
  EntityDeletedMessageConsumer();
  ~EntityDeletedMessageConsumer() override = default;

  void onMessageReceived(const IMessage &message) override;

  private:
};

} // namespace bsgo
