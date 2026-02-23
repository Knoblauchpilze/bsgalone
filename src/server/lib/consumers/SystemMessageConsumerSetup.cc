
#include "SystemMessageConsumerSetup.hh"

#include "JoinShipMessageConsumer.hh"
#include "LoginMessageConsumer.hh"
#include "LoginService.hh"
#include "LogoutMessageConsumer.hh"
#include "PlayerService.hh"
#include "SignupMessageConsumer.hh"
#include "SignupService.hh"
#include "SystemService.hh"

namespace bsgo {

void createSystemMessageConsumers(bsgalone::core::IMessageQueue &inputMessagesQueue,
                                  SystemQueueMap systemQueues,
                                  bsgalone::core::IMessageQueue *const outputMessagesQueue)
{
  Repositories repositories{};

  auto signupService = std::make_unique<SignupService>(repositories);
  inputMessagesQueue.addListener(
    std::make_unique<SignupMessageConsumer>(std::move(signupService), outputMessagesQueue));

  auto loginService = std::make_unique<LoginService>(repositories);
  inputMessagesQueue.addListener(std::make_unique<LoginMessageConsumer>(std::move(loginService),
                                                                        systemQueues,
                                                                        outputMessagesQueue));

  auto systemService = std::make_shared<SystemService>(repositories);
  inputMessagesQueue.addListener(
    std::make_unique<LogoutMessageConsumer>(systemService, systemQueues, outputMessagesQueue));

  auto playerService = std::make_unique<PlayerService>(repositories);
  inputMessagesQueue.addListener(
    std::make_unique<JoinShipMessageConsumer>(std::move(playerService), outputMessagesQueue));
}

} // namespace bsgo
