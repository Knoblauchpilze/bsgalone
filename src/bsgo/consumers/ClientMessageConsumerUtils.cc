
#include "ClientMessageConsumerUtils.hh"

#include "TargetMessageConsumer.hh"
#include "VelocityMessageConsumer.hh"

namespace bsgo {

void createMessageConsumers(IMessageQueue &inputMessagesQueue,
                            IMessageQueue *const outputMessagesQueue,
                            const Services &services)
{
  auto velocity = std::make_unique<VelocityMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(velocity));

  auto target = std::make_unique<TargetMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(target));
}

} // namespace bsgo
