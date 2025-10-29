
#include "OutpostListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

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

auto OutpostListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_systemDbId);

  core::serialize(out, m_outpostsData);

  return out;
}

bool OutpostListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_systemDbId);

  ok &= core::deserialize(in, m_outpostsData);

  return ok;
}

auto OutpostListMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<OutpostListMessage>(m_systemDbId, m_outpostsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
