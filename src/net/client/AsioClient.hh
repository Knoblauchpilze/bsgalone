
#pragma once

#include "AsioContext.hh"
#include "ConnectionStatus.hh"
#include "CoreObject.hh"
#include "IEventBus.hh"
#include "IEventListener.hh"
#include "ReadingSocket.hh"
#include "SocketShPtr.hh"
#include "WritingSocket.hh"

namespace net::details {

class AsioClient : public core::CoreObject,
                   public std::enable_shared_from_this<AsioClient>,
                   public IEventListener
{
  public:
  AsioClient(IEventBusShPtr eventBus);
  ~AsioClient() override = default;

  /// @brief - Starts the client, instructing to asynchronously attempt to connect to
  /// the server.
  /// In case the connection is successful, a client connected event will be published
  /// to the event bus. In case the connection fails, no event will be published to the
  /// event bus.
  /// Currently it is not possible to know when a connection attempt failed. If it is
  /// needed in the future it could be added.
  /// @param context - the context to use to register the connection task
  /// @param url - the URL of the server to connect to
  /// @param port - the port on which the server is listening on
  void connect(AsioContext &context, const std::string &url, const int port);

  /// @brief - Synchronously disconnect the client by closing the underlying socket.
  /// This method will block until the socket is really disconnected.
  /// Calling this method before calling `connect` will fail.
  void disconnect();

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
  auto trySend(std::vector<char> bytes) -> std::optional<MessageId>;

  private:
  /// @brief - Defines the status of the connection. This reflects the progress of the
  /// connection to the remote server. Depending on its value calls to certain methods
  /// might fail due to an invalid state.
  std::atomic<ConnectionStatus> m_status{ConnectionStatus::DISCONNECTED};

  /// @brief - Event bus used to communicate events to the outside world. This includes the
  /// information when the client successfully connects to a remote server or when the
  /// connection is lost.
  IEventBusShPtr m_eventBus{};

  /// @brief - Event bus used to receive messages published by the socket held by the client.
  /// Those events can be interpreted by the client before being published to the external bus.
  /// This is useful to have an internal validation before communicating the data to the
  /// outside world. For example it can be used to interpret a failure to read the data as a
  /// disconnection from the server.
  /// In the future it could also be used as a first gate to attempt to reconnect for example.
  IEventBusShPtr m_internalBus{};

  std::atomic_bool m_registeredToInternalBus{};

  /// @brief - Holds the next message identifier to assign to an outgoing message. This value
  /// is incremented each time a new message is sent.
  std::atomic<MessageId> m_nextMessageId{0};

  /// @brief - Used to protect the access to the socket, the reading and writing socket and
  // to the condition variable.
  std::mutex m_locker{};

  /// @brief - Used to communicate that a disconnection has been detected. This is mainly
  /// used to make the `disconnect` method blocking until the socket is effectively disconnected.
  std::condition_variable m_notifier{};

  /// @brief - Holds the raw socket established by this client. This points to a connection to a
  /// remote server.
  /// This socket is used by the reader and writer objects.
  SocketShPtr m_socket{};

  /// @brief - Holds the client identifier received from the remote server. This value is set
  /// when the `connect` method reaches it conclusion without error. Its value is meaningless
  /// otherwise.
  /// There's currently no good way to know if it is set with a correct value or not. This
  /// is not needed at the moment but could be improved in the future.
  ClientId m_clientId{};

  /// @brief - Holds the reader to interact with the socket attached to the client. It is always
  /// waiting on new data as soon as the `connect` method has been called.
  ReadingSocketShPtr m_reader{};

  /// @brief - Holds the writer allowing to send data to the socket. It can only be used after
  /// the `connect` method has been called.
  WritingSocketShPtr m_writer{};

  void onConnectionEstablished(const std::error_code &code, const asio::ip::tcp::endpoint &endpoint);
  void onDataReceived(const std::error_code &code, const std::size_t contentLength);

  /// @brief - Once the connection has been setup, this method can be used to setup the reading
  /// and writing socket to wrap the interaction with it. After calling this, the socket will
  /// be instrumented and the internal event bus will start receiving events representing what
  /// happens on the socket (such as data received or sent).
  /// The client identifier owned by this object will be associated with all events produced.
  void setupConnection();

  void handleConnectionFailure(const IEvent &event);
};

using AsioClientShPtr = std::shared_ptr<AsioClient>;

} // namespace net::details
