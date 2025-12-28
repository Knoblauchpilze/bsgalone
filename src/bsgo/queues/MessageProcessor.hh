
#pragma once

#include "CoreObject.hh"
#include "IMessage.hh"
#include <deque>
#include <functional>
#include <mutex>
#include <optional>

namespace bsgo {

using MessageHandler = std::function<void(const IMessage &)>;

class MessageProcessor : public core::CoreObject
{
  public:
  MessageProcessor(const std::string &onBehalfOfName,
                   std::deque<IMessagePtr> &messages,
                   std::mutex &locker,
                   MessageHandler handler);

  void processMessages(const std::optional<int> &amount);

  private:
  std::mutex &m_locker;
  std::deque<IMessagePtr> &m_messages;
  MessageHandler m_handler{};

  auto acquireAndClearMessages() -> std::deque<IMessagePtr>;
  void requeueMessages(std::deque<IMessagePtr> &&messages);
};

} // namespace bsgo
