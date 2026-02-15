
#include "TestOutputNetworkAdapter.hh"

namespace test {

bool TestOutputNetworkAdapter::MessageData::operator==(const MessageData &rhs) const
{
  return clientId == rhs.clientId && data == rhs.data;
}

void TestOutputNetworkAdapter::sendMessage(const net::ClientId clientId,
                                           const bsgalone::core::IMessage &message)
{
  const std::lock_guard guard(m_locker);
  m_messages.emplace_back(clientId, message.clone());
}

void TestOutputNetworkAdapter::sendMessage(const bsgalone::core::IMessage &message)
{
  const std::lock_guard guard(m_locker);
  m_messages.emplace_back(std::optional<net::ClientId>{}, message.clone());
}

auto TestOutputNetworkAdapter::messages() const -> const std::vector<MessageData> &
{
  return m_messages;
}

void TestOutputNetworkAdapter::clearMessages()
{
  const std::lock_guard guard(m_locker);
  return m_messages.clear();
}

} // namespace test
