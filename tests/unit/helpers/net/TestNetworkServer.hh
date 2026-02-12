
#pragma once

#include "INetworkServer.hh"
#include <mutex>

namespace test {

class TestNetworkServer : public net::INetworkServer
{
  public:
  TestNetworkServer()           = default;
  ~TestNetworkServer() override = default;

  void start(const int port) override;
  void stop() override;

  auto trySend(const net::ClientId clientId, std::vector<char> bytes)
    -> std::optional<net::MessageId> override;

  struct MessageData
  {
    net::ClientId clientId{};
    std::vector<char> data{};

    bool operator==(const MessageData &rhs) const;
  };

  /// @brief - Returns the accumulated messages so far. Note that this method
  /// is **not thread-safe**.
  /// It is intended to be used after all messages have been successfully added
  /// to the server.
  /// @return - the list of accumulated messages
  auto messages() const -> const std::vector<MessageData> &;

  void clearMessages();

  private:
  std::mutex m_locker{};
  net::MessageId m_nextMessageId{};
  std::vector<MessageData> m_messages{};
};

} // namespace test
