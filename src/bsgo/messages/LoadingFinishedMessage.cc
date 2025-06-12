
#include "LoadingFinishedMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

LoadingFinishedMessage::LoadingFinishedMessage()
  : NetworkMessage(MessageType::LOADING_FINISHED)
{}

auto LoadingFinishedMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  return out;
}

bool LoadingFinishedMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  return ok;
}

auto LoadingFinishedMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<LoadingFinishedMessage>();
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
