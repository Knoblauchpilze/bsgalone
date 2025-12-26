
#include "DataReader.hh"

// TODO: Should be removed.
#include <iostream>

namespace test {

constexpr auto TEMP_BUFFER_SIZE_IN_BYTES = 10;

DataReader::DataReader(net::SocketShPtr socket)
  : m_socket(std::move(socket))
  , m_incomingDataTempBuffer(TEMP_BUFFER_SIZE_IN_BYTES, 0)
{}

auto DataReader::create(net::SocketShPtr socket) -> DataReaderShPtr
{
  std::cout << "[data reader] registering read on creation\n";
  auto reader = std::make_shared<DataReader>(socket);
  reader->registerRead();
  std::cout << "[data reader] done registering read on creation\n";
  return reader;
}

auto DataReader::read() -> std::vector<char>
{
  const std::lock_guard guard(m_inboxLock);
  return m_inbox;
}

void DataReader::onDataReceived(const std::error_code code, const std::size_t contentLength)
{
  std::cout << "[data reader] received " << contentLength << " byte(s)\n";
  if (code)
  {
    if (code != asio::error::eof)
    {
      throw std::runtime_error("Received code " + std::to_string(code.value()) + " (message: \""
                               + code.message() + "\") while reading data");
    }
  }

  const std::lock_guard guard(m_inboxLock);
  std::move(std::begin(m_incomingDataTempBuffer),
            std::begin(m_incomingDataTempBuffer) + contentLength,
            std::back_inserter(m_inbox));

  std::cout << "[data reader] total content is " << m_inbox.size() << " byte(s)\n";

  registerRead();
}

void DataReader::registerRead()
{
  std::cout << "[data reader] registering read\n";
  m_socket->async_read_some(asio::buffer(m_incomingDataTempBuffer.data(),
                                         m_incomingDataTempBuffer.size()),
                            std::bind(&DataReader::onDataReceived,
                                      shared_from_this(),
                                      std::placeholders::_1,
                                      std::placeholders::_2));
}

} // namespace test
