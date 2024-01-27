
#include "MessageConsumerUtils.hh"

#include "DockMessageConsumer.hh"

namespace bsgo {

void createMessageConsumers(IMessageQueue &inputMessagesQueue,
                            IMessageQueue *const outputMessagesQueue,
                            const Services &services)
{
  inputMessagesQueue.addListener(
    std::make_unique<DockMessageConsumer>(services, outputMessagesQueue));
}

} // namespace bsgo
