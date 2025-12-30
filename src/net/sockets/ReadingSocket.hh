
#pragma once

#include "CoreObject.hh"
#include "IEventBus.hh"
#include "SocketShPtr.hh"
#include <memory>
#include <vector>

namespace net::details {

class ReadingSocket;
using ReadingSocketShPtr = std::shared_ptr<ReadingSocket>;

class ReadingSocket : public core::CoreObject, public std::enable_shared_from_this<ReadingSocket>
{
  public:
  ReadingSocket(SocketShPtr socket, IEventBus *eventBus);
  ~ReadingSocket() override = default;

  void connect();

  static auto fromSocket(SocketShPtr socket, IEventBus *eventBus) -> ReadingSocketShPtr;

  private:
  SocketShPtr m_socket{};
  std::atomic_bool m_socketActive{true};

  std::vector<char> m_incomingDataTempBuffer{};

  IEventBus *m_eventBus{};

  void registerReadingTaskToAsio();

  void onDataReceived(const std::error_code &code, const std::size_t contentLength);
  void publishDataReceivedEvent(const std::size_t contentLength);
};

} // namespace net::details
