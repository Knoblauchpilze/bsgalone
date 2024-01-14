
#include "AbstractMessage.hh"

namespace bsgo {

AbstractMessage::AbstractMessage(const MessageType &type)
  : IMessage(str(type))
  , m_messageType(type)
{
  addModule(str(type));
}

auto AbstractMessage::type() const -> MessageType
{
  return m_messageType;
}

auto AbstractMessage::operator<<(std::ostream &out) const -> std::ostream &
{
  out << m_messageType;
  return out;
}

auto AbstractMessage::operator>>(std::istream &in) -> std::istream &
{
  in >> m_messageType;
  return in;
}

} // namespace bsgo
