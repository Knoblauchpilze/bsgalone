
#pragma once

#include "Connection.hh"
#include "IMessageQueue.hh"
#include <core_utils/CoreObject.hh>
#include <deque>
#include <memory>

namespace bsgo {

class NetworkMessageQueue : public IMessageQueue, public utils::CoreObject
{
  public:
  NetworkMessageQueue(IMessageQueuePtr localQueue);
  ~NetworkMessageQueue() override = default;

  void registerToConnection(net::Connection &connection);

  void pushMessage(IMessagePtr message) override;
  void addListener(IMessageListener *listener) override;
  bool empty() override;

  void processMessages(const std::optional<int> &amount = {}) override;

  private:
  IMessageQueuePtr m_localQueue{};

  auto onDataReceived(const net::ConnectionId connectionId, const std::deque<char> &data) -> int;
};

using NetworkMessageQueuePtr = std::unique_ptr<NetworkMessageQueue>;

} // namespace bsgo
