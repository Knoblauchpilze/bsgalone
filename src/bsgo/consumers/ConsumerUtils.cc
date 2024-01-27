
#include "ConsumerUtils.hh"

#include "EquipMessageConsumer.hh"
#include "HangarMessageConsumer.hh"
#include "JumpCancelledMessageConsumer.hh"
#include "JumpMessageConsumer.hh"
#include "JumpRequestedMessageConsumer.hh"
#include "LoginMessageConsumer.hh"
#include "PurchaseMessageConsumer.hh"
#include "SignupMessageConsumer.hh"
#include "SlotMessageConsumer.hh"
#include "TargetMessageConsumer.hh"
#include "VelocityMessageConsumer.hh"

namespace bsgo {

void registerAllConsumersToQueue(IMessageQueue &inputMessagesQueue,
                                 IMessageQueue *const outputMessagesQueue,
                                 const Services &services)
{
  auto jump = std::make_unique<JumpMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(jump));

  auto jumpCancelled = std::make_unique<JumpCancelledMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(jumpCancelled));

  auto jumpRequested = std::make_unique<JumpRequestedMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(jumpRequested));

  auto slot = std::make_unique<SlotMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(slot));

  auto velocity = std::make_unique<VelocityMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(velocity));

  auto hangar = std::make_unique<HangarMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(hangar));

  auto login = std::make_unique<LoginMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(login));

  auto signup = std::make_unique<SignupMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(signup));

  auto target = std::make_unique<TargetMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(target));

  auto purchase = std::make_unique<PurchaseMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(purchase));

  auto equip = std::make_unique<EquipMessageConsumer>(services, outputMessagesQueue);
  inputMessagesQueue.addListener(std::move(equip));
}

} // namespace bsgo
