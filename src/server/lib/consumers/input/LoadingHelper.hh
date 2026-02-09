
#pragma once

#include "CoreObject.hh"
#include "IMessageQueue.hh"
#include "SystemQueues.hh"

namespace bsgo {

class LoadingHelper : public core::CoreObject
{
  public:
  LoadingHelper(SystemQueueMap systemQueues, IMessageQueue *const outputMessageQueue);
  ~LoadingHelper() override = default;

  void publishLoadingMessages(const Uuid clientId,
                              const Uuid playerDbId,
                              const Uuid systemDbId) const;

  private:
  SystemQueueMap m_systemQueues{};
  IMessageQueue *const m_outputMessageQueue{};
};

} // namespace bsgo
