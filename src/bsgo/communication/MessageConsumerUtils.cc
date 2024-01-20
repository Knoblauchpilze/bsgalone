
#include "MessageConsumerUtils.hh"

#include "DockMessageConsumer.hh"
#include "EquipMessageConsumer.hh"
#include "HangarMessageConsumer.hh"
#include "JumpMessageConsumer.hh"
#include "LoginMessageConsumer.hh"
#include "PurchaseMessageConsumer.hh"
#include "SlotMessageConsumer.hh"
#include "TargetMessageConsumer.hh"
#include "VelocityMessageConsumer.hh"

namespace bsgo {

auto registerAllConsumersToQueue(IMessageQueue *const messageQueue, const Services &services)
  -> std::vector<IMessageListenerPtr>
{
  std::vector<IMessageListenerPtr> out{};

  auto jump = std::make_unique<JumpMessageConsumer>(services, messageQueue);
  messageQueue->addListener(jump.get());
  out.emplace_back(std::move(jump));

  auto dock = std::make_unique<DockMessageConsumer>(services, messageQueue);
  messageQueue->addListener(dock.get());
  out.emplace_back(std::move(dock));

  auto slot = std::make_unique<SlotMessageConsumer>(services, messageQueue);
  messageQueue->addListener(slot.get());
  out.emplace_back(std::move(slot));

  auto velocity = std::make_unique<VelocityMessageConsumer>(services, messageQueue);
  messageQueue->addListener(velocity.get());
  out.emplace_back(std::move(velocity));

  auto hangar = std::make_unique<HangarMessageConsumer>(services, messageQueue);
  messageQueue->addListener(hangar.get());
  out.emplace_back(std::move(hangar));

  auto login = std::make_unique<LoginMessageConsumer>(services, messageQueue);
  messageQueue->addListener(login.get());
  out.emplace_back(std::move(login));

  auto target = std::make_unique<TargetMessageConsumer>(services, messageQueue);
  messageQueue->addListener(target.get());
  out.emplace_back(std::move(target));

  auto purchase = std::make_unique<PurchaseMessageConsumer>(services, messageQueue);
  messageQueue->addListener(purchase.get());
  out.emplace_back(std::move(purchase));

  auto equip = std::make_unique<EquipMessageConsumer>(services, messageQueue);
  messageQueue->addListener(equip.get());
  out.emplace_back(std::move(equip));

  return out;
}

} // namespace bsgo
