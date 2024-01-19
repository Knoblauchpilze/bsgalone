
#pragma once

#include "IMessage.hh"
#include <core_utils/CoreObject.hh>
#include <mutex>
#include <vector>

namespace bsgo {

class SynchronizedMessageQueue : public utils::CoreObject
{
  public:
  SynchronizedMessageQueue();
  ~SynchronizedMessageQueue() override = default;

  void pushMessage(IMessagePtr message);

  private:
  std::mutex m_locker{};
  std::vector<IMessagePtr> m_messages{};
};

} // namespace bsgo
