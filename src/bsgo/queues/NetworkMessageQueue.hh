
#pragma once

#include "Connection.hh"
#include "CoreObject.hh"
#include "DataReceivedEvent.hh"
#include "IEventBus.hh"
#include "IEventListener.hh"
#include "IMessageQueue.hh"
#include <deque>
#include <memory>
#include <unordered_map>
#include <vector>

namespace bsgo {

class NetworkMessageQueue : public IMessageQueue, public net::IEventListener, public core::CoreObject
{
  public:
  NetworkMessageQueue(IMessageQueuePtr synchronizedQueue);
  ~NetworkMessageQueue() override = default;

  bool isEventRelevant(const net::EventType &type) const override;
  void onEventReceived(const net::IEvent &event) override;

  void pushMessage(IMessagePtr message) override;
  void addListener(IMessageListenerPtr listener) override;
  bool empty() override;

  void processMessages(const std::optional<int> &amount = {}) override;

  private:
  IMessageQueuePtr m_synchronizedQueue{};

  struct ClientData
  {
    std::deque<char> bytes{};
  };

  std::mutex m_locker{};
  std::unordered_map<net::ClientId, ClientData> m_pendingData{};

  void registerPendingData(const net::DataReceivedEvent &event);
  auto onDataReceived(const net::ClientId clientId) -> int;
  void feedMessagesToQueue(std::vector<IMessagePtr> &&messages);
  void removePendingData(const net::ClientId clientId, const int processed);
};

using NetworkMessageQueuePtr = std::unique_ptr<NetworkMessageQueue>;

} // namespace bsgo
