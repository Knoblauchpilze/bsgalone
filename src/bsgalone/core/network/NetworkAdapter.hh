
#pragma once

#include "ClientId.hh"
#include "DataReceivedEvent.hh"
#include "IEventListener.hh"
#include "IMessage.hh"
#include "IMessageParser.hh"
#include "IMessageQueue.hh"
#include <deque>

namespace bsgalone::core {

class NetworkAdapter : public net::IEventListener
{
  public:
  NetworkAdapter(IMessageQueueShPtr queue, IMessageParserPtr parser);
  ~NetworkAdapter() override = default;

  bool isEventRelevant(const net::EventType &type) const override;
  void onEventReceived(const net::IEvent &event) override;

  private:
  IMessageQueueShPtr m_queue{};
  IMessageParserPtr m_parser{};

  struct ClientData
  {
    std::deque<char> bytes{};
  };

  /// @brief - Protects access to the pending data.
  std::mutex m_locker{};

  /// @brief - Holds the pending data received without any client identifier
  /// attached to it. This is used in the client applications where the data
  /// comes without an identifier as the server does not communicate it.
  ClientData m_noClientData{};

  /// @brief - Holds the pending data for each client. Each time new data is
  /// received, the adapter adds it to the corresponding client data and tries
  /// to build messages out of it.
  /// When a message is built, the corresponding data is removed from the
  /// pending list.
  std::unordered_map<net::ClientId, ClientData> m_pendingData{};

  void registerPendingData(const net::DataReceivedEvent &event);
  auto onDataReceived(const std::optional<net::ClientId> &maybeClientId) -> int;
  void feedMessagesToQueue(const std::optional<net::ClientId> &maybeClientId,
                           std::vector<IMessagePtr> &&messages);
  void removePendingData(const std::optional<net::ClientId> &maybeClientId, const int processed);
};

} // namespace bsgalone::core
