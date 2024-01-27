
#include "ServerMessageConsumerUtils.hh"

#include "DockMessageConsumer.hh"
#include "LoginMessageConsumer.hh"
#include "SignupMessageConsumer.hh"

namespace bsgo {

void createMessageConsumers(IMessageQueue &inputMessagesQueue,
                            IMessageQueue *const outputMessagesQueue,
                            const Services &services)
{
  inputMessagesQueue.addListener(
    std::make_unique<SignupMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue.addListener(
    std::make_unique<LoginMessageConsumer>(services, outputMessagesQueue));

  inputMessagesQueue.addListener(
    std::make_unique<DockMessageConsumer>(services, outputMessagesQueue));
}

} // namespace bsgo
