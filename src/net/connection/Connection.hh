
#pragma once

#include <asio/asio.hpp>
#include <core_utils/CoreObject.hh>
#include <memory>

namespace net {

class Connection : public utils::CoreObject
{
  public:
  Connection(asio::io_context &context);
  ~Connection() override = default;

  bool isConnected() const;

  private:
  asio::ip::tcp::socket m_socket;
};

using ConnectionPtr = std::unique_ptr<Connection>;

} // namespace net
