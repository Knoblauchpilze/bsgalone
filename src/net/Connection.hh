
#pragma once

#include <asio/asio.hpp>
#include <core_utils/CoreObject.hh>

namespace net {

class Connection : public utils::CoreObject
{
  public:
  Connection(asio::ip::tcp::socket socket);
  ~Connection() override = default;

  bool isConnected() const;

  private:
  asio::ip::tcp::socket m_socket;
};

} // namespace net
