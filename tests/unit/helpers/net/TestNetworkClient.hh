
#pragma once

#include "INetworkClient.hh"
#include <mutex>

namespace test {

class TestNetworkClient : public net::INetworkClient
{
  public:
  TestNetworkClient()           = default;
  ~TestNetworkClient() override = default;

  void connect(const std::string &url, const int port) override;
  void disconnect() override;

  auto trySend(std::vector<char> bytes) -> std::optional<net::MessageId> override;

  struct MessageData
  {
    net::MessageId messageId{};
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
