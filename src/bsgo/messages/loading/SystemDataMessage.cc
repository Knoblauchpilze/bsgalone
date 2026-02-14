
#include "SystemDataMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

SystemDataMessage::SystemDataMessage()
  : NetworkMessage(MessageType::SYSTEM_DATA)
{}

SystemDataMessage::SystemDataMessage(const SystemTickData &tickData)
  : NetworkMessage(MessageType::SYSTEM_DATA)
  , m_tickData(tickData)
{}

auto SystemDataMessage::getTickData() const -> SystemTickData
{
  return m_tickData;
}

auto SystemDataMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_tickData);

  return out;
}

bool SystemDataMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_tickData);

  return ok;
}

auto SystemDataMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<SystemDataMessage>(m_tickData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
