
#pragma once

#include "CoreObject.hh"
#include "IMessage.hh"
#include <deque>
#include <functional>
#include <mutex>
#include <optional>

namespace bsgo {

using MessageHandler = std::function<void(const bsgalone::core::IMessage &)>;

class MessageProcessor : public core::CoreObject
{
  public:
  MessageProcessor(const std::string &onBehalfOfName,
                   std::deque<bsgalone::core::IMessagePtr> &messages,
                   std::mutex &locker,
                   MessageHandler handler);

  void processMessages();

  private:
  std::mutex &m_locker;
  std::deque<bsgalone::core::IMessagePtr> &m_messages;
  MessageHandler m_handler{};

  auto acquireAndClearMessages() -> std::deque<bsgalone::core::IMessagePtr>;
  void printMessagesInfo(const std::deque<bsgalone::core::IMessagePtr> &messages) const;
};

} // namespace bsgo
