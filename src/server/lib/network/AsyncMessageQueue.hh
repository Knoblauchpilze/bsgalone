
#pragma once

#include "IMessageQueue.hh"
#include <atomic>
#include <condition_variable>
#include <core_utils/CoreObject.hh>
#include <memory>
#include <thread>

namespace bsgo {

class AsyncMessageQueue : public IMessageQueue, public utils::CoreObject
{
  public:
  AsyncMessageQueue(IMessageQueuePtr messageQueue);
  ~AsyncMessageQueue() override;

  void pushMessage(IMessagePtr message) override;
  void addListener(IMessageListener *listener) override;
  bool empty() override;

  void processMessages(const std::optional<int> &amount = {}) override;

  private:
  IMessageQueuePtr m_messageQueue{};

  std::atomic_bool m_running{false};
  std::thread m_queueThread{};

  std::mutex m_messageLocker{};
  std::condition_variable m_messageNotifier{};

  void initialize();
  void asyncMessageProcessing();
};

using AsyncMessageQueuePtr = std::unique_ptr<AsyncMessageQueue>;

} // namespace bsgo
