
#pragma once

#include "CoreObject.hh"
#include "IMessageQueue.hh"
#include "SystemQueues.hh"

namespace bsgalone::server {

class LoadingHelper : public ::core::CoreObject
{
  public:
  LoadingHelper(SystemQueueMap systemQueues, core::IMessageQueue *const outputMessageQueue);
  ~LoadingHelper() override = default;

  void publishLoadingMessages(const core::Uuid clientId,
                              const core::Uuid playerDbId,
                              const core::Uuid systemDbId) const;

  private:
  SystemQueueMap m_systemQueues{};
  core::IMessageQueue *const m_outputMessageQueue{};
};

} // namespace bsgalone::server
