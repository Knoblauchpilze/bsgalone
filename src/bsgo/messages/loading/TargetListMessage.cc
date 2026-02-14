
#include "TargetListMessage.hh"
#include "SerializationUtils.hh"

namespace bsgo {

TargetListMessage::TargetListMessage()
  : NetworkMessage(MessageType::TARGET_LIST)
{}

TargetListMessage::TargetListMessage(const Uuid systemDbId,
                                     const std::vector<TargetData> &targetsData)
  : NetworkMessage(MessageType::TARGET_LIST)
  , m_systemDbId(systemDbId)
  , m_targetsData(targetsData)
{}

auto TargetListMessage::getSystemDbId() const -> Uuid
{
  return m_systemDbId;
}

auto TargetListMessage::getTargetsData() const -> const std::vector<TargetData> &
{
  return m_targetsData;
}

auto TargetListMessage::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, m_messageType);
  core::serialize(out, m_clientId);

  core::serialize(out, m_systemDbId);

  core::serialize(out, m_targetsData);

  return out;
}

bool TargetListMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= core::deserialize(in, m_messageType);
  ok &= core::deserialize(in, m_clientId);

  ok &= core::deserialize(in, m_systemDbId);

  ok &= core::deserialize(in, m_targetsData);

  return ok;
}

auto TargetListMessage::clone() const -> bsgalone::core::IMessagePtr
{
  auto clone = std::make_unique<TargetListMessage>(m_systemDbId, m_targetsData);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
