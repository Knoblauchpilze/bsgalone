
#include "Message.hh"

namespace net {

Message::Message(const MessageType type)
  : m_type(type)
{}

auto Message::type() const -> MessageType
{
  return m_type;
}

auto Message::size() const -> std::size_t
{
  return messageTypeSize() + m_body.size();
}

bool Message::hasBody() const
{
  return !m_body.empty();
}

} // namespace net
