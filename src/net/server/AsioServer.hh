
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

  IEventBusShPtr m_eventBus{};

  /// @brief - Event bus used to receive messages published by the sockets held by the server.
  /// Those events can be interpreted by the server before being published to the external bus.
  /// This allows to for example remove the sockets from the local maps when an error is reported
  /// on one of the connection (e.g. read failure).
  /// It is declared **after** the `m_eventBus` so that it gets destroyed **before**. This is
  /// necessary so that the last internal events received can make it to the event bus when the
  /// server is destroyed. Otherwise there is a segmentation fault as the event bus is deleted
  /// before the last internal events are processed.
  IEventBusShPtr m_internalBus{};

  std::atomic<ClientId> m_nextClientId{0};
  std::mutex m_connectionsLocker{};
  std::unordered_map<ClientId, ReadingSocketShPtr> m_readers{};
  std::unordered_map<ClientId, WritingSocketShPtr> m_writers{};

  void registerToAsio();

  void onConnectionRequest(const std::error_code &code, asio::ip::tcp::socket socket);
  auto registerConnection(asio::ip::tcp::socket rawSocket) -> ClientId;
  void handleConnectionFailure(const IEvent &event);
};

using AsioServerPtr = std::unique_ptr<AsioServer>;

} // namespace net::details
