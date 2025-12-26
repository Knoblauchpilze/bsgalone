
#include "ReadingSocket.hh"
#include "AsioUtils.hh"

namespace net::details {
// Maximum amount of bytes that can be read at once.
constexpr auto KILOBYTES                              = 1'024;
constexpr auto INCOMING_DATA_MAX_BUFFER_SIZE_IN_BYTES = 10 * KILOBYTES;

ReadingSocket::ReadingSocket(SocketShPtr socket)
  : core::CoreObject("reading")
  , m_socket(std::move(socket))
  , m_incomingDataTempBuffer(INCOMING_DATA_MAX_BUFFER_SIZE_IN_BYTES, 0)
{
  setService("socket");
}

void ReadingSocket::connect()
{
  if (!m_socketActive.load())
  {
    error("Cannot connect closed socket");
  }

  registerReadingTaskToAsio();
}

auto ReadingSocket::read() -> std::vector<char>
{
  if (!m_socketActive.load())
  {
    error("Cannot read from closed socket");
  }

  const std::lock_guard guard(m_inboxLock);

  std::vector<char> out{};
  out.reserve(m_inbox.size());
  std::copy(m_inbox.begin(), m_inbox.end(), std::back_inserter(out));

  return out;
}

auto ReadingSocket::fromSocket(SocketShPtr socket) -> ReadingSocketShPtr
{
  return std::make_shared<ReadingSocket>(std::move(socket));
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

    m_socketActive.store(false);

    return;
  }

  verbose("Received " + std::to_string(contentLength) + " byte(s) on "
          + str(m_socket->remote_endpoint()));

  copyTempDataToReceivedData(contentLength);
  registerReadingTaskToAsio();
}

void ReadingSocket::copyTempDataToReceivedData(const std::size_t contentLength)
{
  const std::lock_guard guard(m_inboxLock);

  std::move(std::begin(m_incomingDataTempBuffer),
            std::begin(m_incomingDataTempBuffer) + contentLength,
            std::back_inserter(m_inbox));
}

} // namespace net::details
