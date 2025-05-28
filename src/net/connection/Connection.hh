
#pragma once

#include "ConnectionType.hh"
#include "CoreObject.hh"
#include <asio.hpp>
#include <atomic>
#include <deque>
#include <memory>
#include <optional>
#include <vector>

namespace net {

using ConnectionId        = int;
using DataReceivedHandler = std::function<int(const ConnectionId, const std::deque<char> &)>;
using DisconnectHandler   = std::function<void(const ConnectionId)>;

class Connection : public core::CoreObject, public std::enable_shared_from_this<Connection>
{
  public:
  Connection(const std::string &url, const int port, asio::io_context &context);
  Connection(asio::ip::tcp::socket &&socket);
  ~Connection() override = default;

  auto str() const -> std::string;
  auto id() const -> ConnectionId;
  auto type() const -> ConnectionType;
  bool isConnected() const;

  void connect();
  void setDataHandler(const DataReceivedHandler &dataHandler);
  void setDisconnectHandler(const DisconnectHandler &disconnectHandler);

  template<typename T>
  void send(const T &message);

  private:
  static std::atomic<ConnectionId> NEXT_ID;

  ConnectionId m_id;
  ConnectionType m_type{};
  asio::ip::tcp::socket m_socket;
  std::optional<asio::ip::tcp::resolver::results_type> m_endpoints{};

  struct MessageToSend
  {
    std::vector<char> data{};
  };
  using MessageToSendPtr = std::unique_ptr<MessageToSend>;

  std::mutex m_dataLock{};
  bool m_sendingData{false};
  std::deque<MessageToSendPtr> m_messagesToSend{};

  std::vector<char> m_incomingDataTempBuffer{};
  std::deque<char> m_partialMessageData{};
  std::optional<DataReceivedHandler> m_dataHandler{};
  std::optional<DisconnectHandler> m_disconnectHandler{};

  Connection(asio::ip::tcp::socket &&socket, const ConnectionType type);

  void registerReadingTaskToAsio();
  void registerConnectingTaskToAsio();
  void registerMessageSendingTaskToAsio();

  void registerMessageToSend(MessageToSendPtr &&message);

  void onConnectionEstablished(const std::error_code &code, const asio::ip::tcp::endpoint &endpoint);
  void onDataReceived(const std::error_code &code, const std::size_t contentLength);
  void onDataSent(const std::error_code &code, const std::size_t contentLength);
};

// This is needed to register shared_from_this in asio handlers.
using ConnectionShPtr = std::shared_ptr<Connection>;

} // namespace net

#include "Connection.hxx"
