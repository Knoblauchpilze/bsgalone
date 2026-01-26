
#pragma once

#include "ClientManager.hh"
#include "CoreObject.hh"
#include "IMessageQueue.hh"
#include "SystemQueues.hh"

namespace bsgo {

class LoadingHelper : public core::CoreObject
{
  public:
  LoadingHelper(ClientManagerShPtr clientManager,
                SystemQueueMap systemQueues,
                IMessageQueue *const outputMessageQueue);
  ~LoadingHelper() override = default;

  void publishLoadingMessages(const Uuid clientId, const Uuid playerDbId) const;

  private:
  ClientManagerShPtr m_clientManager{};
  SystemQueueMap m_systemQueues{};
  IMessageQueue *const m_outputMessageQueue{};
};

} // namespace bsgo
