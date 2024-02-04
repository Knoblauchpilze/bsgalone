
#include "LootMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

LootMessage::LootMessage()
  : NetworkMessage(MessageType::LOOT)
{}

LootMessage::LootMessage(const Uuid playerDbId, const Uuid resourceDbId, const float amount)
  : NetworkMessage(MessageType::LOOT)
  , m_playerDbId(playerDbId)
  , m_resourceDbId(resourceDbId)
  , m_amount(amount)
{}

auto LootMessage::getPlayerDbId() const -> Uuid
{
  return m_playerDbId;
}

auto LootMessage::getResourceDbId() const -> Uuid
{
  return m_resourceDbId;
}

auto LootMessage::amount() const -> float
{
  return m_amount;
}

auto LootMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);

  utils::serialize(out, m_playerDbId);
  utils::serialize(out, m_resourceDbId);
  utils::serialize(out, m_amount);

  return out;
}

bool LootMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);

  ok &= utils::deserialize(in, m_playerDbId);
  ok &= utils::deserialize(in, m_resourceDbId);
  ok &= utils::deserialize(in, m_amount);

  return ok;
}

auto LootMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<LootMessage>(m_playerDbId, m_resourceDbId, m_amount);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
