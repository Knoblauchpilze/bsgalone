
#pragma once

#include "SocketPtr.hh"
#include <memory>

namespace test {

class DataReader;
using DataReaderShPtr = std::shared_ptr<DataReader>;

class DataReader : public std::enable_shared_from_this<DataReader>
{
  public:
  DataReader(net::SocketShPtr socket);
  ~DataReader() = default;

  static auto create(net::SocketShPtr socket) -> DataReaderShPtr;

  auto read() -> std::vector<char>;

  private:
  net::SocketShPtr m_socket{};
  std::vector<char> m_inbox{};
  std::vector<char> m_incomingDataTempBuffer{};
  std::mutex m_inboxLock{};

  void onDataReceived(const std::error_code &code, const std::size_t contentLength);
  void registerRead();
};

} // namespace test
