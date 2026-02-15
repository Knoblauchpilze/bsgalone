
#pragma once

#include "IMessageQueue.hh"
#include <mutex>

namespace test {

class TestMessageQueue : public bsgalone::core::IMessageQueue
{
  public:
  TestMessageQueue()           = default;
  ~TestMessageQueue() override = default;

  void pushMessage(bsgalone::core::IMessagePtr message) override;
  void addListener(bsgalone::core::IMessageListenerPtr /*listener*/) override;
  bool empty() override;
  void processMessages() override;

  /// @brief - Returns the accumulated messages so far. Note that this method
  /// is **not thread-safe**.
  /// It is intended to be used after all events have been successfully added
  /// to the queue.
  /// @return - the list of accumulated messages
  auto messages() const -> const std::vector<bsgalone::core::IMessagePtr> &;

  void clearMessages();

  private:
  std::mutex m_locker{};
  std::vector<bsgalone::core::IMessagePtr> m_messages{};
};

} // namespace test
