
#pragma once

#include "ClientManager.hh"
#include "IMessageQueue.hh"
#include "Uuid.hh"
#include <core_utils/CoreObject.hh>
#include <deque>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace bsgo {

class BroadcastMessageQueue : public IMessageQueue, public utils::CoreObject
{
  public:
  BroadcastMessageQueue(ClientManagerShPtr clientManager);
  ~BroadcastMessageQueue() override = default;

  void pushMessage(IMessagePtr message) override;
  void addListener(IMessageListenerPtr listener) override;
  bool empty() override;

  void processMessages(const std::optional<int> &amount = {}) override;

  private:
  std::mutex m_locker{};
  std::deque<IMessagePtr> m_messages{};
  ClientManagerShPtr m_clientManager{};

  void processMessage(const IMessage &message);

  void sendMessageToClient(const Uuid clientId, const IMessage &message);
  void broadcastMessage(const IMessage &message);
};

using BroadcastMessageQueuePtr = std::unique_ptr<BroadcastMessageQueue>;

} // namespace bsgo
