
#pragma once

#include "ClientId.hh"
#include "CoreObject.hh"
#include "IEventBus.hh"
#include "SocketShPtr.hh"
#include <deque>
#include <memory>
#include <vector>

namespace net::details {

class WritingSocket : public core::CoreObject, public std::enable_shared_from_this<WritingSocket>
{
  public:
  WritingSocket(const ClientId clientId, SocketShPtr socket, IEventBusShPtr eventBus);
  ~WritingSocket() override = default;

  void send(std::vector<char> bytes);

  private:
  struct MessageToSend
  {
    std::vector<char> content{};
  };
  using MessageToSendPtr = std::unique_ptr<MessageToSend>;

  ClientId m_clientId{};
  SocketShPtr m_socket{};
  std::atomic_bool m_socketActive{true};

  std::mutex m_outboxLock{};
  bool m_writingTaskRegistered{false};
  std::deque<MessageToSendPtr> m_outbox{};

  IEventBusShPtr m_eventBus{};

  void pushMessageToOutbox(std::vector<char> bytes);
  void registerWritingTaskToAsio();

  void onDataSent(const std::error_code &code, const std::size_t contentLength);
  void publishDataSentEvent();
  void popFirstMessageInOutbox();
};

using WritingSocketShPtr = std::shared_ptr<WritingSocket>;

} // namespace net::details
