
#include "WritingSocket.hh"
#include "ClientDisconnectedEvent.hh"
#include "DataSentEvent.hh"

#include <iostream>

namespace net::details {

WritingSocket::WritingSocket(const ClientId clientId, SocketShPtr socket, IEventBusShPtr eventBus)
  : core::CoreObject("writing")
  , m_clientId(clientId)
  , m_socket(std::move(socket))
  , m_eventBus(eventBus)
{
  setService("net");
  addModule("socket");

  if (m_socket == nullptr)
  {
    throw std::invalid_argument("Expected non null socket");
  }
  if (m_eventBus == nullptr)
  {
    throw std::invalid_argument("Expected non null event bus");
  }
}

void WritingSocket::send(std::vector<char> bytes)
{
  if (!m_socketActive.load())
  {
    error("Cannot write to closed socket");
  }

  pushMessageToOutbox(bytes);
  registerWritingTaskToAsio();
}

void WritingSocket::pushMessageToOutbox(std::vector<char> bytes)
{
  const std::lock_guard guard(m_outboxLock);
  auto message = std::make_unique<MessageToSend>(std::move(bytes));
  m_outbox.emplace_back(std::move(message));
}

void WritingSocket::registerWritingTaskToAsio()
{
  const std::lock_guard guard(m_outboxLock);

  if (m_writingTaskRegistered)
  {
    // Already one writing task, do not register a new one until the
    // existing one finishes.
    return;
  }
  if (m_outbox.empty())
  {
    return;
  }

  const auto &message = m_outbox.front();

  asio::async_write(*m_socket,
                    asio::buffer(message->content.data(), message->content.size()),
                    std::bind(&WritingSocket::onDataSent,
                              shared_from_this(),
                              std::placeholders::_1,
                              std::placeholders::_2));

  m_writingTaskRegistered = true;
}

void WritingSocket::onDataSent(const std::error_code &code, const std::size_t contentLength)
{
  if (code)
  {
    warn("Error detected when sending data on connection",
         code.message() + " (code: " + std::to_string(code.value()) + ")");

    m_socketActive.store(false);

    auto event = std::make_unique<ClientDisconnectedEvent>(m_clientId);
    m_eventBus->pushEvent(std::move(event));

    return;
  }

  info("received code " + std::to_string(code.value()) + ", msg: " + code.message()
       + ", length: " + std::to_string(contentLength));

  publishDataSentEvent();
  popFirstMessageInOutbox();
  registerWritingTaskToAsio();
}

void WritingSocket::publishDataSentEvent()
{
  auto event = std::make_unique<DataSentEvent>(m_clientId);
  m_eventBus->pushEvent(std::move(event));
}

void WritingSocket::popFirstMessageInOutbox()
{
  const std::lock_guard guard(m_outboxLock);
  m_outbox.pop_front();
  m_writingTaskRegistered = false;
}

} // namespace net::details
