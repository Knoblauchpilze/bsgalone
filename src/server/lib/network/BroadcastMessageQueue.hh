
#pragma once

#include "Connection.hh"
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
  BroadcastMessageQueue();
  ~BroadcastMessageQueue() override = default;

  void registerClient(const Uuid clientId, net::ConnectionShPtr connection);

  void pushMessage(IMessagePtr message) override;
  void addListener(IMessageListenerPtr listener) override;
  bool empty() override;

  void processMessages(const std::optional<int> &amount = {}) override;

  private:
  std::mutex m_locker{};
  std::deque<IMessagePtr> m_messages{};
  std::unordered_map<Uuid, net::ConnectionShPtr> m_clients{};

  void processMessage(const IMessage &message);
};

using BroadcastMessageQueuePtr = std::unique_ptr<BroadcastMessageQueue>;

} // namespace bsgo
