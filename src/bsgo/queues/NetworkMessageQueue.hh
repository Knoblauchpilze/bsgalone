
#pragma once

#include "Connection.hh"
#include "IMessageQueue.hh"
#include <core_utils/CoreObject.hh>
#include <deque>
#include <memory>
#include <vector>

namespace bsgo {

class NetworkMessageQueue : public IMessageQueue, public utils::CoreObject
{
  public:
  NetworkMessageQueue(IMessageQueuePtr synchronizedQueue);
  ~NetworkMessageQueue() override = default;

  void registerToConnection(net::Connection &connection);

  void pushMessage(IMessagePtr message) override;
  void addListener(IMessageListenerPtr listener) override;
  bool empty() override;

  void processMessages(const std::optional<int> &amount = {}) override;

  private:
  IMessageQueuePtr m_synchronizedQueue{};

  auto onDataReceived(const net::ConnectionId connectionId, const std::deque<char> &data) -> int;
  void feedMessagesToQueue(std::vector<IMessagePtr> &&messages);
};

using NetworkMessageQueuePtr = std::unique_ptr<NetworkMessageQueue>;

} // namespace bsgo
