
#include "ReadingSocket.hh"
#include "AsioUtils.hh"

#include <iostream>

namespace net::details {
// Maximum amount of bytes that can be read at once.
constexpr auto KILOBYTES                              = 1'024;
constexpr auto INCOMING_DATA_MAX_BUFFER_SIZE_IN_BYTES = 10 * KILOBYTES;

ReadingSocket::ReadingSocket(SocketShPtr socket)
  : core::CoreObject("reading")
  , m_socket(std::move(socket))
  , m_incomingDataTempBuffer(INCOMING_DATA_MAX_BUFFER_SIZE_IN_BYTES, 0)
{
  std::cout << "[reading socket] reading socket constructor\n";
  setService("socket");
}

void ReadingSocket::connect()
{
  std::cout << "[reading socket] connecting reading socket in class\n";
  registerReadingTaskToAsio();
  std::cout << "[reading socket] connected reading socket\n";
}

auto ReadingSocket::read() -> std::vector<char>
{
  std::cout << "[reading socket] reading socket read\n";
  const std::lock_guard guard(m_inboxLock);

  std::cout << "[reading socket] getting inbox data\n";
  std::vector<char> out{};
  out.reserve(m_inbox.size());

  std::copy(m_inbox.begin(), m_inbox.end(), std::back_inserter(out));

  std::cout << "[reading socket] got " << out.size() << " byte(s)\n";

  return out;
}

void ReadingSocket::registerReadingTaskToAsio()
{
  m_socket->async_read_some(asio::buffer(m_incomingDataTempBuffer.data(),
                                         m_incomingDataTempBuffer.size()),
                            std::bind(&ReadingSocket::onDataReceived,
                                      shared_from_this(),
                                      std::placeholders::_1,
                                      std::placeholders::_2));
}

void ReadingSocket::onDataReceived(const std::error_code code, const std::size_t contentLength)
{
  if (code)
  {
    warn("Error detected when receiving data from connection",
         code.message() + " (code: " + std::to_string(code.value()) + ")");

    m_socket->close();
    return;
  }

  std::cout << "[reading socket] reading socket received " << contentLength << " byte(s)\n";
  verbose("Received " + std::to_string(contentLength) + " byte(s) on "
          + str(m_socket->remote_endpoint()));

  copyTempDataToReceivedData(contentLength);
  std::cout << "[reading socket] done copying data to local array\n";
  registerReadingTaskToAsio();
  std::cout << "[reading socket] registered to asio again\n";
}

void ReadingSocket::copyTempDataToReceivedData(const std::size_t contentLength)
{
  const std::lock_guard guard(m_inboxLock);

  std::move(std::begin(m_incomingDataTempBuffer),
            std::begin(m_incomingDataTempBuffer) + contentLength,
            std::back_inserter(m_inbox));
}

} // namespace net::details
