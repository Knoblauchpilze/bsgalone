
#include "ConnectionMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

ConnectionMessage::ConnectionMessage()
  : AbstractMessage(MessageType::CONNECTION)
{}

auto ConnectionMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);

  return out;
}

bool ConnectionMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);

  return ok;
}

auto ConnectionMessage::clone() const -> IMessagePtr
{
  return std::make_unique<ConnectionMessage>();
}

} // namespace bsgo
