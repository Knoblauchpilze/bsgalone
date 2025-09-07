
#pragma once

#include "ClientManager.hh"
#include "CoreObject.hh"
#include "IMessageQueue.hh"
#include "SystemProcessor.hh"

namespace bsgo {

class LoadingHelper : public core::CoreObject
{
  public:
  LoadingHelper(ClientManagerShPtr clientManager,
                SystemProcessorMap systemProcessors,
                IMessageQueue *const outputMessageQueue);
  ~LoadingHelper() override = default;

  void publishLoadingMessages(const Uuid clientId, const Uuid playerDbId) const;

  private:
  ClientManagerShPtr m_clientManager{};
  SystemProcessorMap m_systemProcessors{};
  IMessageQueue *const m_outputMessageQueue{};
};

} // namespace bsgo
