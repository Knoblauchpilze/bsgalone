
#include "WritingSocket.hh"
#include "AsioUtils.hh"

namespace net::details {

WritingSocket::WritingSocket(SocketShPtr socket)
  : core::CoreObject("writing")
  , m_socket(std::move(socket))
{
  setService("socket");
}

void WritingSocket::send(std::vector<char> bytes)
{
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
  if (m_outbox.empty())
  {
    return;
  }
  if (m_writingTaskRegistered)
  {
    // Already one writing task, do not register a new one until the
    // existing one finishes.
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

void WritingSocket::onDataSent(const std::error_code code, const std::size_t contentLength)
{
  if (code)
  {
    warn("Error detected when sending data on connection",
         code.message() + " (code: " + std::to_string(code.value()) + ")");

    m_socket->close();
    return;
  }

  verbose("Sent " + std::to_string(contentLength) + " byte(s) on "
          + str(m_socket->remote_endpoint()));

  popFirstMessageInOutbox();
  registerWritingTaskToAsio();
}

void WritingSocket::popFirstMessageInOutbox()
{
  const std::lock_guard guard(m_outboxLock);
  m_outbox.pop_front();
  m_writingTaskRegistered = false;
}

} // namespace net::details
