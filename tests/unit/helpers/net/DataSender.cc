
#include "DataSender.hh"

// TODO: Should be removed.
#include <iostream>

namespace test {

DataSender::DataSender(const std::string &bytes)
  : DataSender(std::vector(bytes.begin(), bytes.end()))
{}

DataSender::DataSender(std::vector<char> bytes)
  : m_bytes(std::move(bytes))
{}

void DataSender::writeTo(asio::ip::tcp::socket &socket)
{
  auto result = m_sent.get_future();

  std::cout << "[data sender] writing data\n";
  asio::async_write(socket,
                    asio::buffer(m_bytes.data(), m_bytes.size()),
                    std::bind(&DataSender::onDataSent,
                              shared_from_this(),
                              std::placeholders::_1,
                              std::placeholders::_2));

  std::cout << "[data sender] writing done\n";
  result.get();
  std::cout << "[data sender] writing confirmed\n";
}

auto DataSender::create(const std::string &data) -> DataSenderShPtr
{
  return std::make_shared<DataSender>(data);
}

void DataSender::onDataSent(const std::error_code &code, const std::size_t contentLength)
{
  std::cout << "[data sender] received some info for send\n";
  if (code)
  {
    throw std::runtime_error("Received code " + std::to_string(code.value()) + " (message: \""
                             + code.message() + "\") while sending data");
  }
  if (contentLength != m_bytes.size())
  {
    throw std::runtime_error("Sent only " + std::to_string(contentLength)
                             + " byte(s) to connection, expected to send "
                             + std::to_string(m_bytes.size()));
  }
  std::cout << "[data sender] received confirmation for send\n";
  m_sent.set_value(true);
}

} // namespace test
