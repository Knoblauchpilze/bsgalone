
#include "WritingSocket.hh"
#include "AsioUtils.hh"

#include <iostream>

namespace net::details {

WritingSocket::WritingSocket(SocketShPtr socket)
  : core::CoreObject("writing")
  , m_socket(std::move(socket))
{
  setService("socket");
}

void WritingSocket::send(std::vector<char> bytes)
{
  if (!m_socketActive.load())
  {
    error("Cannot write to closed socket");
  }

  std::cout << "[writing] pushing message\n";
  pushMessageToOutbox(bytes);
  std::cout << "[writing] register writing\n";
  registerWritingTaskToAsio();
  std::cout << "[writing] registered\n";
}

auto WritingSocket::fromSocket(SocketShPtr socket) -> WritingSocketShPtr
{
  return std::make_shared<WritingSocket>(std::move(socket));
}

void WritingSocket::pushMessageToOutbox(std::vector<char> bytes)
{
  const std::lock_guard guard(m_outboxLock);
  auto message = std::make_unique<MessageToSend>(std::move(bytes));
  m_outbox.emplace_back(std::move(message));
}

bool WritingSocket::checkConnectivity()
{
  if (!m_socketActive.load())
  {
    return false;
  }

  std::error_code code;
  m_socket->remote_endpoint(code);

  std::cout << "[writing] remote: " << code.value() << ", " << code.message() << "\n";

  if (code)
  {
    m_socketActive.store(false);
    std::cout << "[writing] socket is closed\n";
    return false;
  }

  return true;
}

void WritingSocket::registerWritingTaskToAsio()
{
  const std::lock_guard guard(m_outboxLock);
  if (!checkConnectivity())
  {
    std::cout << "[writing] do not register\n";
    return;
  }
  if (m_outbox.empty())
  {
    std::cout << "[writing] outbox empty\n";
    return;
  }
  if (m_writingTaskRegistered)
  {
    // Already one writing task, do not register a new one until the
    // existing one finishes.
    std::cout << "[writing] task registered\n";
    return;
  }

  const auto &message = m_outbox.front();

  std::cout << "[writing] async write scheduling\n";
  asio::async_write(*m_socket,
                    asio::buffer(message->content.data(), message->content.size()),
                    std::bind(&WritingSocket::onDataSent,
                              shared_from_this(),
                              std::placeholders::_1,
                              std::placeholders::_2));
  std::cout << "[writing] async write scheduled\n";

  m_writingTaskRegistered = true;
}

void WritingSocket::onDataSent(const std::error_code &code, const std::size_t /*contentLength*/)
{
  std::cout << "[writing] got result: " << code.value() << ", " << code.message() << "\n";
  if (code)
  {
    warn("Error detected when sending data on connection",
         code.message() + " (code: " + std::to_string(code.value()) + ")");

    m_socketActive.store(false);

    return;
  }

  std::cout << "[writing] post push actions\n";
  popFirstMessageInOutbox();
  registerWritingTaskToAsio();
  std::cout << "[writing] finished handling\n";
}

void WritingSocket::popFirstMessageInOutbox()
{
  const std::lock_guard guard(m_outboxLock);
  m_outbox.pop_front();
  m_writingTaskRegistered = false;
}

} // namespace net::details
