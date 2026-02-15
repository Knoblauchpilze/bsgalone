
#pragma once

#include "IOutputNetworkAdapter.hh"
#include <mutex>

namespace test {

class TestOutputNetworkAdapter : public bsgalone::core::IOutputNetworkAdapter
{
  public:
  TestOutputNetworkAdapter()           = default;
  ~TestOutputNetworkAdapter() override = default;

  void sendMessage(const net::ClientId clientId, const bsgalone::core::IMessage &message) override;
  void sendMessage(const bsgalone::core::IMessage &message) override;

  struct MessageData
  {
    std::optional<net::ClientId> clientId{};
    bsgalone::core::IMessagePtr data{};

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
  std::vector<MessageData> m_messages{};
};

} // namespace test
