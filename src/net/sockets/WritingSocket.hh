
#pragma once

#include "ClientId.hh"
#include "CoreObject.hh"
#include "IEventBus.hh"
#include "MessageId.hh"
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

  void send(const MessageId messageId, std::vector<char> bytes);

  private:
  struct MessageToSend
  {
    MessageId id{};
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

  void pushMessageToOutbox(const MessageId messageId, std::vector<char> bytes);
  void registerWritingTaskToAsio();

  void onDataSent(const std::error_code &code, const std::size_t contentLength);
  void publishDataSentEvent();
  void popFirstMessageInOutbox();

  /// @brief - Returns the identifier of the first message to send. In case no
  /// message is registered to be sent, an error will be raised.
  /// @return - the identifier of the message to send.
  auto getFirstMessageId() -> MessageId;
};

using WritingSocketShPtr = std::shared_ptr<WritingSocket>;

} // namespace net::details
