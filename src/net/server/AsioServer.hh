
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

  /// @brief - Starts the server, instructing to begin listening for incoming connections
  /// on the port provided at construction time. This effectively opens the server to be
  /// contacted by external clients and start managing TCP connections.
  /// Calling this method twice will raise an exception.
  void start();

  bool isEventRelevant(const EventType &type) const override;
  void onEventReceived(const IEvent &event) override;

  /// @brief - Used to send a message to the client identified by the input identifier.
  /// Sending the data is queued for processing and will be send asynchronously.
  /// Some notes:
  ///   - in case the client identifier cannot be matched to an active connection, it will
  ///     be discarded and an empty message identifier will be returned.
  ///   - in case the `bytes` to send are empty, the function will return early and an
  ///     empty message identifier will be returned.
  /// It is possible to receive information about the data being sent by listening for
  /// `DataSentEvent` with the corresponding message identifier.
  /// @param clientId - the identifier of the client to send the message to
  /// @param bytes - the content of the message
  /// @return - an identifier for the message or empty if the client does not exist
  auto trySend(const ClientId clientId, std::vector<char> bytes) -> std::optional<MessageId>;

  private:
  /// @brief - The raw acceptor managed by the asio library. This handles the low level
  /// connection and socket management for the server.
  asio::ip::tcp::acceptor m_acceptor;

  /// @brief - Controls whether the `start` method was already called and prevents it to
  /// be called again.
  std::atomic_bool m_registered{false};

  /// @brief - Event bus used to communicate events to the outside world. This includes the
  /// information about clients connected or disconnected, but also in general about data
  /// being written or received.
  /// Only the server writes to this bus, the underlying sockets are only given access to
  /// the internal bus.
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

  /// @brief - Holds the next client identifier to assign to an incoming connection. This value
  /// is incremented each time a new connection is received.
  std::atomic<ClientId> m_nextClientId{0};

  /// @brief - Holds the next message identifier to assign to an outgoing message. This value
  /// is incremented each time a new message is sent.
  std::atomic<MessageId> m_nextMessageId{0};

  /// @brief - This locker is used to protect access to the three attributes below: `m_sockets`,
  /// `m_readers` and `m_writers`.
  std::mutex m_connectionsLocker{};

  /// @brief - Holds the list of all active sockets currently connected to the server. Each of
  /// those socket is also attached to exactly one reader and one writer.
  /// This is used to keep track of the sockets and be able to close them easily when the server
  /// is being destroyed.
  std::vector<SocketShPtr> m_sockets{};

  /// @brief - Holds the list of readers that are currently waiting on data for an open socket.
  /// This map always contains exactly as many entries as the `m_sockets` array.
  std::unordered_map<ClientId, ReadingSocketShPtr> m_readers{};

  /// @brief - Holds the list of writers that can send data to remote clients. This map always
  /// contains exactly as many entries as the `m_sockets` array.
  std::unordered_map<ClientId, WritingSocketShPtr> m_writers{};

  void registerToAsio();

  void onConnectionRequest(const std::error_code &code, asio::ip::tcp::socket socket);
  auto registerConnection(asio::ip::tcp::socket rawSocket) -> ClientId;
  void handleConnectionFailure(const IEvent &event);
};

using AsioServerPtr = std::unique_ptr<AsioServer>;

} // namespace net::details
