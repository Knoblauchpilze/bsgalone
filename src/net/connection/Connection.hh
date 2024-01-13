
#pragma once

#include "ConnectionType.hh"
#include "Message.hh"
#include <asio/asio.hpp>
#include <core_utils/CoreObject.hh>
#include <deque>
#include <memory>
#include <vector>

namespace net {

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

  private:
  ConnectionType m_type;
  asio::ip::tcp::socket m_socket;

  Connection(asio::ip::tcp::socket &&socket, const ConnectionType type);

  void registerToAsio();

  static constexpr auto INCOMING_DATA_BUFFER_SIZE = 50 * 1'024;
  std::vector<char> m_incomingDataTempBuffer{};
  std::deque<char> m_partialMessageData{};

  void registerServerConnectionToAsio();
  void onDataReceived(const std::error_code &code, const std::size_t contentLength);
  void handlePartialData();
};

using ConnectionPtr = std::unique_ptr<Connection>;

} // namespace net
