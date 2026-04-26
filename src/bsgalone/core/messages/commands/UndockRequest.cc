
#include "UndockRequest.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

UndockRequest::UndockRequest()
  : IMessage(MessageType::UNDOCK_REQUEST)
{}

UndockRequest::UndockRequest(const Uuid playerDbId)
  : IMessage(MessageType::UNDOCK_REQUEST)
  , m_playerDbId(playerDbId)
{}

auto UndockRequest::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto UndockRequest::clone() const -> IMessagePtr
{
  return std::make_unique<UndockRequest>(m_playerDbId);
}

auto UndockRequest::readFromStream(std::istream &in) -> std::optional<IMessagePtr>
{
  UndockRequest message;

  bool ok{true};
  ok &= ::core::deserialize(in, message.m_type);
  ok &= ::core::deserialize(in, message.m_playerDbId);

  if (!ok)
  {
    return {};
  }

  return message.clone();
}

auto operator<<(std::ostream &out, const UndockRequest &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_playerDbId);

  return out;
}

} // namespace bsgalone::core
