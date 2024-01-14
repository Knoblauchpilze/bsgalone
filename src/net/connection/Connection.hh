
#pragma once

#include "ConnectionType.hh"
#include "Message.hh"
#include <asio/asio.hpp>
#include <core_utils/CoreObject.hh>
#include <deque>
#include <memory>
#include <optional>
#include <vector>

namespace net {

using DataReceivedHandler = std::function<int(const std::deque<char> &)>;

class Connection : public utils::CoreObject
{
  public:
  Connection(asio::io_context &context);
  Connection(asio::ip::tcp::socket &&socket);
  ~Connection() override = default;

  auto str() const -> std::string;
  auto type() const -> ConnectionType;
  bool isConnected() const;

  void activate();
  void setDataHandler(const DataReceivedHandler &dataHandler);

  private:
  ConnectionType m_type;
  asio::ip::tcp::socket m_socket;

  Connection(asio::ip::tcp::socket &&socket, const ConnectionType type);

  void registerToAsio();

  static constexpr auto INCOMING_DATA_BUFFER_SIZE = 50 * 1'024;
  std::vector<char> m_incomingDataTempBuffer{};
  std::deque<char> m_partialMessageData{};
  std::optional<DataReceivedHandler> m_dataHandler{};

  void registerServerConnectionToAsio();
  void onDataReceived(const std::error_code &code, const std::size_t contentLength);
};

using ConnectionPtr = std::unique_ptr<Connection>;

} // namespace net
