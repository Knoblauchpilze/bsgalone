
#pragma once

#include "ConnectionType.hh"
#include "Message.hh"
#include <asio/asio.hpp>
#include <core_utils/CoreObject.hh>
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

  static constexpr auto MESSAGE_HEADER_DATA_SIZE = messageTypeSize();
  std::vector<char> m_incomingDataTempBuffer{MESSAGE_HEADER_DATA_SIZE, 0};

  void registerServerConnectionToAsio();
  void onDataReceived(const std::error_code &code, const std::size_t contentLength);
};

using ConnectionPtr = std::unique_ptr<Connection>;

} // namespace net
