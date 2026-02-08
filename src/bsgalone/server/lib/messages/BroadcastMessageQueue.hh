
#pragma once

#include "ClientManager.hh"
#include "CoreObject.hh"
#include "IMessageQueue.hh"
#include "INetworkServer.hh"
#include "Uuid.hh"
#include <deque>
#include <memory>
#include <mutex>
#include <vector>

namespace bsgalone::server {

class BroadcastMessageQueue : public bsgo::IMessageQueue, public core::CoreObject
{
  public:
  BroadcastMessageQueue(ClientManagerShPtr clientManager, net::INetworkServerShPtr server);
  ~BroadcastMessageQueue() override = default;

  void pushMessage(bsgo::IMessagePtr message) override;
  void addListener(bsgo::IMessageListenerPtr listener) override;
  bool empty() override;

  void processMessages() override;

  private:
  std::mutex m_locker{};
  std::deque<bsgo::IMessagePtr> m_messages{};
  ClientManagerShPtr m_clientManager{};
  net::INetworkServerShPtr m_server{};

  void processMessage(const bsgo::IMessage &message);

  void sendMessageToClient(const net::ClientId clientId, const bsgo::IMessage &message);
  void broadcastMessage(const bsgo::IMessage &message);

  auto tryDetermineClientId(const bsgo::IMessage &message) const -> std::optional<bsgo::Uuid>;
  auto tryDetermineSystemIds(const bsgo::IMessage &message) const -> std::vector<bsgo::Uuid>;
};

using BroadcastMessageQueuePtr = std::unique_ptr<BroadcastMessageQueue>;

} // namespace bsgalone::server
