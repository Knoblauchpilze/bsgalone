
#include "OutpostListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

OutpostListMessage::OutpostListMessage()
  : NetworkMessage(MessageType::OUTPOST_LIST)
{}

OutpostListMessage::OutpostListMessage(const Uuid systemDbId,
                                       const std::vector<OutpostData> &outpostsData)
  : NetworkMessage(MessageType::OUTPOST_LIST)
  , m_systemDbId(systemDbId)
  , m_outpostsData(outpostsData)
{}

auto OutpostListMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

auto OutpostListMessage::getOutpostsData() const -> const std::vector<OutpostData> &
{
  return m_outpostsData;
}

auto OutpostListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<OutpostListMessage>(m_systemDbId, m_outpostsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const OutpostListMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_systemDbId);
  ::core::serialize(out, message.m_outpostsData);

  return out;
}

auto operator>>(std::istream &in, OutpostListMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_systemDbId);
  ::core::deserialize(in, message.m_outpostsData);

  return in;
}

} // namespace bsgalone::core
