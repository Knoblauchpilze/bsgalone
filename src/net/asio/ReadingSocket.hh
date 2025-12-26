
#pragma once

#include "CoreObject.hh"
#include "SocketPtr.hh"
#include <asio.hpp>
#include <deque>
#include <memory>

namespace net::details {

class ReadingSocket;
using ReadingSocketShPtr = std::shared_ptr<ReadingSocket>;

class ReadingSocket : public core::CoreObject, public std::enable_shared_from_this<ReadingSocket>
{
  public:
  ReadingSocket(SocketShPtr socket);
  ~ReadingSocket() override = default;

  void connect();
  auto read() -> std::vector<char>;

  static auto fromSocket(SocketShPtr socket) -> ReadingSocketShPtr;

  private:
  SocketShPtr m_socket{};
  std::atomic_bool m_socketActive{true};

  std::vector<char> m_incomingDataTempBuffer{};

  std::mutex m_inboxLock{};
  std::deque<char> m_inbox{};

  void registerReadingTaskToAsio();

  void onDataReceived(const std::error_code code, const std::size_t contentLength);
  void copyTempDataToReceivedData(const std::size_t contentLength);
};

} // namespace net::details
