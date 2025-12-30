
#pragma once

#include "CoreObject.hh"
#include "SocketPtr.hh"
#include <asio.hpp>
#include <deque>
#include <memory>

namespace net::details {

class WritingSocket;
using WritingSocketShPtr = std::shared_ptr<WritingSocket>;

class WritingSocket : public core::CoreObject, public std::enable_shared_from_this<WritingSocket>
{
  public:
  WritingSocket(SocketShPtr socket);
  ~WritingSocket() override = default;

  bool isConnected() const;

  void send(std::vector<char> bytes);

  static auto fromSocket(SocketShPtr socket) -> WritingSocketShPtr;

  private:
  struct MessageToSend
  {
    std::vector<char> content{};
  };
  using MessageToSendPtr = std::unique_ptr<MessageToSend>;

  SocketShPtr m_socket{};
  std::atomic_bool m_socketActive{true};

  std::mutex m_outboxLock{};
  bool m_writingTaskRegistered{false};
  std::deque<MessageToSendPtr> m_outbox{};

  void pushMessageToOutbox(std::vector<char> bytes);
  bool checkConnectivity();
  void registerWritingTaskToAsio();

  void onDataSent(const std::error_code &code, const std::size_t contentLength);
  void popFirstMessageInOutbox();
};

} // namespace net::details
