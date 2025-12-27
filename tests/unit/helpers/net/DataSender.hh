
#pragma once

#include <asio.hpp>
#include <memory>
#include <string>
#include <vector>

namespace test {

class DataSender;
using DataSenderShPtr = std::shared_ptr<DataSender>;

class DataSender : public std::enable_shared_from_this<DataSender>
{
  public:
  DataSender(const std::string &bytes);
  DataSender(std::vector<char> bytes);

  ~DataSender() = default;

  void writeTo(asio::ip::tcp::socket &socket);

  static auto create(const std::string &data) -> DataSenderShPtr;

  private:
  std::vector<char> m_bytes{};
  std::promise<bool> m_sent{};

  void onDataSent(const std::error_code &code, const std::size_t contentLength);
};

} // namespace test
