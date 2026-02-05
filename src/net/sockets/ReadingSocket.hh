
#pragma once

#include "ClientId.hh"
#include "CoreObject.hh"
#include "IEventBus.hh"
#include "SocketShPtr.hh"
#include <memory>
#include <optional>
#include <vector>

namespace net::details {

class ReadingSocket : public core::CoreObject, public std::enable_shared_from_this<ReadingSocket>
{
  public:
  /// @brief - Creates a reading socket using the input raw socket as the source of data.
  /// Whenever a network event happens, the socket will publish the events to the event
  /// bus provided in input.
  /// With this constructor, the events produced will not be assigned a client identifier.
  /// @param socket - the raw socket to read from
  /// @param eventBus - the event bus to use to publish network events
  ReadingSocket(SocketShPtr socket, IEventBusShPtr eventBus);

  /// @brief - Creates a reading socket using the input raw socket as the source of data.
  /// Whenever a network event happens, the socket will publish the events to the event
  /// bus provided in input.
  /// With this constructor, the events produced will be assigned the client idetnfier
  /// given as input.
  /// @param clientId - the identifier of the client for which this socket is reading data
  /// @param socket - the raw socket to read from
  /// @param eventBus - the event bus to use to publish network events
  ReadingSocket(const ClientId clientId, SocketShPtr socket, IEventBusShPtr eventBus);

  ~ReadingSocket() override = default;

  void connect();

  private:
  std::optional<ClientId> m_clientId{};
  SocketShPtr m_socket{};
  std::atomic_bool m_socketConnected{false};
  std::atomic_bool m_socketActive{true};

  std::vector<char> m_incomingDataTempBuffer{};

  IEventBusShPtr m_eventBus{};

  void registerReadingTaskToAsio();

  void onDataReceived(const std::error_code &code, const std::size_t contentLength);
  void publishDataReceivedEvent(const std::size_t contentLength);
};

using ReadingSocketShPtr = std::shared_ptr<ReadingSocket>;

} // namespace net::details
