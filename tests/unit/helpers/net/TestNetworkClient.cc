
#include "TestNetworkClient.hh"

namespace test {

bool TestNetworkClient::MessageData::operator==(const MessageData &rhs) const
{
  return messageId == rhs.messageId && data == rhs.data;
}

void TestNetworkClient::connect(const std::string & /*url*/, const int /*port*/) {}

void TestNetworkClient::disconnect() {}

auto TestNetworkClient::trySend(std::vector<char> bytes) -> std::optional<net::MessageId>
{
  const std::lock_guard guard(m_locker);

  const auto messageId = m_nextMessageId;
  ++m_nextMessageId;

  m_messages.emplace_back(messageId, std::move(bytes));

  return messageId;
}

auto TestNetworkClient::messages() const -> const std::vector<MessageData> &
{
  return m_messages;
}

void TestNetworkClient::clearMessages()
{
  const std::lock_guard guard(m_locker);
  return m_messages.clear();
}

} // namespace test
