
#pragma once

#include "CoreObject.hh"
#include "SocketPtr.hh"
#include <asio.hpp>
#include <deque>
#include <memory>

namespace net::details {

class ReadingSocket : public core::CoreObject, public std::enable_shared_from_this<ReadingSocket>
{
  public:
  ReadingSocket(SocketShPtr socket);
  ~ReadingSocket() override = default;

  auto read() -> std::vector<char>;

  private:
  SocketShPtr m_socket{};

  std::vector<char> m_incomingDataTempBuffer{};

  std::mutex m_inboxLock{};
  std::deque<char> m_inbox{};

  void registerReadingTaskToAsio();

  void onDataReceived(const std::error_code code, const std::size_t contentLength);
  void copyTempDataToReceivedData(const std::size_t contentLength);
};

} // namespace net::details
