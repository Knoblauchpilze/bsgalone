
#pragma once

#include "ClientId.hh"
#include "CoreObject.hh"
#include "IEventBus.hh"
#include "SocketShPtr.hh"
#include <memory>
#include <vector>

namespace net::details {

class ReadingSocket : public core::CoreObject, public std::enable_shared_from_this<ReadingSocket>
{
  public:
  ReadingSocket(const ClientId clientId, SocketShPtr socket, IEventBusShPtr eventBus);
  ~ReadingSocket() override = default;

  void connect();

  private:
  ClientId m_clientId{};
  SocketShPtr m_socket{};
  std::atomic_bool m_socketConnected{false};
  std::atomic_bool m_socketActive{true};

  std::vector<char> m_incomingDataTempBuffer{};

  IEventBusShPtr m_eventBus{};

  void registerReadingTaskToAsio();

  void onDataReceived(const std::error_code &code, const std::size_t contentLength);
  void publishDataReceivedEvent(const std::size_t contentLength);
};

using ReadingSocketPtr = std::unique_ptr<ReadingSocket>;

} // namespace net::details
