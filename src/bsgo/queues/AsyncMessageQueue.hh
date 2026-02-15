
#pragma once

#include "CoreObject.hh"
#include "IMessageQueue.hh"
#include <atomic>
#include <condition_variable>
#include <thread>

namespace bsgo {

class AsyncMessageQueue : public bsgalone::core::IMessageQueue, public core::CoreObject
{
  public:
  AsyncMessageQueue(bsgalone::core::IMessageQueuePtr messageQueue);
  ~AsyncMessageQueue() override;

  void pushMessage(bsgalone::core::IMessagePtr message) override;
  void addListener(bsgalone::core::IMessageListenerPtr listener) override;
  bool empty() override;

  void processMessages() override;

  private:
  bsgalone::core::IMessageQueuePtr m_messageQueue{};

  std::atomic_bool m_running{false};
  std::thread m_queueThread{};

  std::mutex m_messageLocker{};
  std::condition_variable m_messageNotifier{};

  void initialize();
  void asyncMessageProcessing();
};

} // namespace bsgo
