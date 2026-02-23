
#pragma once

#include "AbstractMessageConsumer.hh"
#include "IMessageQueue.hh"
#include "SystemQueues.hh"

namespace bsgo {

class TriageMessageConsumer : public bsgalone::core::AbstractMessageConsumer
{
  public:
  TriageMessageConsumer(SystemQueueMap systemQueues,
                        bsgalone::core::IMessageQueuePtr systemMessageQueue);
  ~TriageMessageConsumer() override = default;

  void onMessageReceived(const bsgalone::core::IMessage &message) override;

  private:
  SystemQueueMap m_systemQueues{};
  bsgalone::core::IMessageQueuePtr m_systemQueue{};

  bool discardMessageWithNoClient(const bsgalone::core::IMessage &message) const;
  bool tryRoutePlayerOrSystemMessage(const bsgalone::core::IMessage &message) const;
  void handleSystemMessage(const bsgalone::core::IMessage &message) const;
  void routeSystemMessage(const Uuid systemDbId, const bsgalone::core::IMessage &message) const;
};

} // namespace bsgo
