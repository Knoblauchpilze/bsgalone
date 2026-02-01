
#pragma once

#include "ClientId.hh"
#include "DataReceivedEvent.hh"
#include "IEventListener.hh"
#include "IMessage.hh"
#include "IMessageQueue.hh"
#include <deque>

namespace bsgalone::core {

class NetworkAdapter : public net::IEventListener
{
  public:
  NetworkAdapter(bsgo::IMessageQueueShPtr queue);
  ~NetworkAdapter() override = default;

  bool isEventRelevant(const net::EventType &type) const override;
  void onEventReceived(const net::IEvent &event) override;

  private:
  bsgo::IMessageQueueShPtr m_queue{};

  struct ClientData
  {
    std::deque<char> bytes{};
  };

  std::mutex m_locker{};
  std::unordered_map<net::ClientId, ClientData> m_pendingData{};

  void registerPendingData(const net::DataReceivedEvent &event);
  auto onDataReceived(const net::ClientId clientId) -> int;
  void feedMessagesToQueue(const net::ClientId clientId, std::vector<bsgo::IMessagePtr> &&messages);
  void removePendingData(const net::ClientId clientId, const int processed);
};

} // namespace bsgalone::core
