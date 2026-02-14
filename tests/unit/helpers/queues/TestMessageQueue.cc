
#include "TestMessageQueue.hh"

namespace test {

void TestMessageQueue::pushMessage(bsgalone::core::IMessagePtr message)
{
  const std::lock_guard guard(m_locker);
  m_messages.push_back(std::move(message));
}

void TestMessageQueue::addListener(bsgo::IMessageListenerPtr /*listener*/)
{
  throw std::runtime_error("Unsupported operation addListener in TestMessageQueue");
}

bool TestMessageQueue::empty()
{
  const std::lock_guard guard(m_locker);
  return m_messages.empty();
}

void TestMessageQueue::processMessages()
{
  throw std::runtime_error("Unsupported operation processMessages in TestMessageQueue");
}

auto TestMessageQueue::messages() const -> const std::vector<bsgalone::core::IMessagePtr> &
{
  return m_messages;
}

void TestMessageQueue::clearMessages()
{
  const std::lock_guard guard(m_locker);
  return m_messages.clear();
}

} // namespace test
