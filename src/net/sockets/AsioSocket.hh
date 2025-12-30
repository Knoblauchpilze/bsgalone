
#pragma once

#include "ISocket.hh"
#include "ReadingSocket.hh"
#include "SocketPtr.hh"
#include "WritingSocket.hh"
#include <memory>

namespace net::details {

class AsioSocket;
using AsioSocketPtr = std::unique_ptr<AsioSocket>;

class AsioSocket : public ISocket
{
  public:
  AsioSocket(SocketShPtr socket);
  ~AsioSocket() override = default;

  static auto create(asio::ip::tcp::socket socket) -> AsioSocketPtr;

  auto endpoint() const -> std::string override;
  bool isConnected() const override;
  void close() override;
  void connect() override;

  void send(std::vector<char> bytes) override;
  auto read() -> std::vector<char> override;

  protected:
  SocketShPtr m_socket{};
  ReadingSocketShPtr m_reader{};
  WritingSocketShPtr m_writer{};
};

} // namespace net::details
