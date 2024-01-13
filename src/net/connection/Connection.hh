
#pragma once

#include "ConnectionType.hh"
#include <asio/asio.hpp>
#include <core_utils/CoreObject.hh>
#include <memory>

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

  private:
  ConnectionType m_type;
  asio::ip::tcp::socket m_socket;

  Connection(asio::ip::tcp::socket &&socket, const ConnectionType type);
};

using ConnectionPtr = std::unique_ptr<Connection>;

} // namespace net
