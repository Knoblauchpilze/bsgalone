
#pragma once

#include "AsioContext.hh"
#include "ClientId.hh"
#include "CoreObject.hh"
#include "IEventBus.hh"
#include "ReadingSocket.hh"
#include "WritingSocket.hh"
#include <asio.hpp>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace net::details {

class AsioServer : public core::CoreObject,
                   public std::enable_shared_from_this<AsioServer>,
                   public IEventListener
{
  public:
  AsioServer(AsioContext &context, const int port, IEventBusShPtr eventBus);
  ~AsioServer() override;

  void start();

  bool isEventRelevant(const EventType &type) const override;
  void onEventReceived(const IEvent &event) override;

  private:
  asio::ip::tcp::acceptor m_acceptor;

  std::atomic_bool m_registered{false};

  std::atomic<ClientId> m_nextClientId{0};
  std::mutex m_connectionsLocker{};
  std::unordered_map<ClientId, ReadingSocketShPtr> m_readers{};
  std::unordered_map<ClientId, WritingSocketShPtr> m_writers{};

  IEventBusShPtr m_eventBus{};

  IEventBusShPtr m_internalBus{};

  void registerToAsio();

  void onConnectionRequest(const std::error_code &code, asio::ip::tcp::socket socket);
  auto registerConnection(asio::ip::tcp::socket rawSocket) -> ClientId;
  void publishClientConnectedEvent(const ClientId clientId);
};

using AsioServerPtr = std::unique_ptr<AsioServer>;

} // namespace net::details
