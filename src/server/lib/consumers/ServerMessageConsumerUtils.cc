
#include "ServerMessageConsumerUtils.hh"

#include "DockMessageConsumer.hh"
#include "EquipMessageConsumer.hh"
#include "HangarMessageConsumer.hh"
#include "JumpCancelledMessageConsumer.hh"
#include "JumpMessageConsumer.hh"
#include "JumpRequestedMessageConsumer.hh"
#include "LoginMessageConsumer.hh"
#include "PurchaseMessageConsumer.hh"
#include "SignupMessageConsumer.hh"
#include "SlotMessageConsumer.hh"

namespace bsgo {

void createMessageConsumers(const ConsumersConfig &config)
{
  config.inputMessagesQueue.addListener(
    std::make_unique<SignupMessageConsumer>(config.services, config.outputMessagesQueue));

  config.inputMessagesQueue.addListener(
    std::make_unique<LoginMessageConsumer>(config.services,
                                           config.outputMessagesQueue,
                                           config.loginCallback));

  config.inputMessagesQueue.addListener(
    std::make_unique<HangarMessageConsumer>(config.services, config.outputMessagesQueue));

  config.inputMessagesQueue.addListener(
    std::make_unique<PurchaseMessageConsumer>(config.services, config.outputMessagesQueue));

  config.inputMessagesQueue.addListener(
    std::make_unique<EquipMessageConsumer>(config.services, config.outputMessagesQueue));

  config.inputMessagesQueue.addListener(
    std::make_unique<DockMessageConsumer>(config.services, config.outputMessagesQueue));

  config.inputMessagesQueue.addListener(
    std::make_unique<SlotMessageConsumer>(config.services, config.outputMessagesQueue));

  config.inputMessagesQueue.addListener(
    std::make_unique<JumpMessageConsumer>(config.services, config.outputMessagesQueue));

  config.inputMessagesQueue.addListener(
    std::make_unique<JumpCancelledMessageConsumer>(config.services, config.outputMessagesQueue));

  config.inputMessagesQueue.addListener(
    std::make_unique<JumpRequestedMessageConsumer>(config.services, config.outputMessagesQueue));
}

} // namespace bsgo
