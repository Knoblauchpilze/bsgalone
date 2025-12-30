
#include "ReadingSocket.hh"
#include "DataReceivedEvent.hh"

namespace net::details {
// Maximum amount of bytes that can be read at once.
constexpr auto KILOBYTES                              = 1'024;
constexpr auto INCOMING_DATA_MAX_BUFFER_SIZE_IN_BYTES = 10 * KILOBYTES;

ReadingSocket::ReadingSocket(SocketShPtr socket, IEventBus *eventBus)
  : core::CoreObject("reading")
  , m_socket(std::move(socket))
  , m_incomingDataTempBuffer(INCOMING_DATA_MAX_BUFFER_SIZE_IN_BYTES, 0)
  , m_eventBus(eventBus)
{
  setService("net");
  addModule("reading");

  if (m_eventBus == nullptr)
  {
    throw std::invalid_argument("Expected non null event bus");
  }
}

void ReadingSocket::connect()
{
  if (!m_socketActive.load())
  {
    error("Cannot connect closed socket");
  }

  // TODO: We should maybe prevent double registration
  registerReadingTaskToAsio();
}

auto ReadingSocket::fromSocket(SocketShPtr socket, IEventBus *eventBus) -> ReadingSocketShPtr
{
  return std::make_shared<ReadingSocket>(std::move(socket), eventBus);
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

void ReadingSocket::onDataReceived(const std::error_code &code, const std::size_t contentLength)
{
  if (code)
  {
    warn("Error detected when receiving data from connection",
         code.message() + " (code: " + std::to_string(code.value()) + ")");

    m_socketActive.store(false);

    return;
  }

  verbose("Received " + std::to_string(contentLength) + " byte(s) on connection ");

  publishDataReceivedEvent(contentLength);
  registerReadingTaskToAsio();
}

void ReadingSocket::publishDataReceivedEvent(const std::size_t contentLength)
{
  if (contentLength == 0)
  {
    return;
  }

  // This portion of code is not concurrent: there's only ever one reading
  // task being registered and this is only called from the onDataReceived
  // callback. Therefore it is safe to just access the temp data buffer.
  std::vector<char> data;
  data.reserve(contentLength);

  std::move(std::begin(m_incomingDataTempBuffer),
            std::begin(m_incomingDataTempBuffer) + contentLength,
            std::back_inserter(data));

  // TODO: Should include a client identifier
  auto event = std::make_unique<DataReceivedEvent>(ClientId{0}, std::move(data));
  m_eventBus->pushEvent(std::move(event));
}

} // namespace net::details
