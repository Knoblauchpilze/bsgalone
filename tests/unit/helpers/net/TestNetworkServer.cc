
#include "TestNetworkServer.hh"

namespace test {

bool TestNetworkServer::MessageData::operator==(const MessageData &rhs) const
{
  return clientId == rhs.clientId && data == rhs.data;
}

void TestNetworkServer::start(const int /*port*/) {}

void TestNetworkServer::stop() {}

auto TestNetworkServer::trySend(const net::ClientId clientId, std::vector<char> bytes)
  -> std::optional<net::MessageId>
{
  const std::lock_guard guard(m_locker);
  m_messages.emplace_back(clientId, std::move(bytes));

  const auto messageId = m_nextMessageId;
  ++m_nextMessageId;

  return messageId;
}

auto TestNetworkServer::messages() const -> const std::vector<MessageData> &
{
  return m_messages;
}

void TestNetworkServer::clearMessages()
{
  const std::lock_guard guard(m_locker);
  return m_messages.clear();
}

} // namespace test
