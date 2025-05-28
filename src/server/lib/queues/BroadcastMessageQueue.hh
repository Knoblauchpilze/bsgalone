
#pragma once

#include "ClientManager.hh"
#include "CoreObject.hh"
#include "IMessageQueue.hh"
#include "Uuid.hh"
#include <deque>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace bsgo {

class BroadcastMessageQueue : public IMessageQueue, public core::CoreObject
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

  auto tryDetermineClientId(const IMessage &message) const -> std::optional<Uuid>;
  auto tryDetermineSystemIds(const IMessage &message) const -> std::vector<Uuid>;
};

using BroadcastMessageQueuePtr = std::unique_ptr<BroadcastMessageQueue>;

} // namespace bsgo
