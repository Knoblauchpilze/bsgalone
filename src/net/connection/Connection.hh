
#pragma once

#include "ConnectionType.hh"
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
  Connection(const std::string &url, const int port, asio::io_context &context);
  Connection(asio::ip::tcp::socket &&socket);
  ~Connection() override = default;

  auto str() const -> std::string;
  auto type() const -> ConnectionType;
  bool isConnected() const;

  void connect();
  void disconnect();
  void setDataHandler(const DataReceivedHandler &dataHandler);

  template<typename T>
  void send(const T &message);

  private:
  ConnectionType m_type;
  asio::ip::tcp::socket m_socket;
  std::optional<asio::ip::tcp::resolver::results_type> m_endpoints{};

  struct MessageToSend
  {
    std::vector<char> data{};
  };
  using MessageToSendPtr = std::unique_ptr<MessageToSend>;

  std::mutex m_dataLock{};
  std::deque<MessageToSendPtr> m_messagesToSend{};

  std::vector<char> m_incomingDataTempBuffer{};
  std::deque<char> m_partialMessageData{};
  std::optional<DataReceivedHandler> m_dataHandler{};

  Connection(asio::ip::tcp::socket &&socket, const ConnectionType type);

  void registerToAsio();
  void registerReadingTaskToAsio();
  void registerConnectingTaskToAsio();
  void registerMessageSendingTaskToAsio();

  void registerMessageToSend(MessageToSendPtr &&message);

  void onConnectionEstablished(const std::error_code &code, const asio::ip::tcp::endpoint &endpoint);
  void onDataReceived(const std::error_code &code, const std::size_t contentLength);
  void onDataSent(const std::error_code &code, const std::size_t contentLength);
};

using ConnectionPtr = std::unique_ptr<Connection>;

} // namespace net

#include "Connection.hxx"
