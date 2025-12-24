
#pragma once

#include "ISocket.hh"
#include "ReadingSocket.hh"
#include "SocketPtr.hh"
#include "WritingSocket.hh"

namespace net::details {

class AsioSocket : public ISocket
{
  public:
  AsioSocket(SocketShPtr socket);
  ~AsioSocket() override = default;

  auto endpoint() const -> std::string override;
  bool isConnected() const override;
  void close() override;

  void send(std::vector<char> bytes) override;
  auto read() -> std::vector<char> override;

  protected:
  SocketShPtr m_socket{};
  ReadingSocket m_reader;
  WritingSocket m_writer;
};

} // namespace net::details
