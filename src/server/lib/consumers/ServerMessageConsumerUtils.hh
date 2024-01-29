
#pragma once

#include "IMessageListener.hh"
#include "IMessageQueue.hh"
#include "LoginCallback.hh"
#include "Services.hh"

namespace bsgo {

struct ConsumersConfig
{
  IMessageQueue &inputMessagesQueue;
  IMessageQueue *const outputMessagesQueue{};
  Services services{};
  PlayerLoginCallback loginCallback{};
};

void createMessageConsumers(const ConsumersConfig &config);

} // namespace bsgo
