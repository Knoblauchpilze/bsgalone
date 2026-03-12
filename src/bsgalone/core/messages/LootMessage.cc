
#include "LootMessage.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

LootMessage::LootMessage()
  : NetworkMessage(MessageType::LOOT)
{}

LootMessage::LootMessage(const Uuid playerDbId, const Uuid resourceDbId, const int amount)
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

auto LootMessage::amount() const -> int
{
  return m_amount;
}

auto LootMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<LootMessage>(m_playerDbId, m_resourceDbId, m_amount);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

auto operator<<(std::ostream &out, const LootMessage &message) -> std::ostream &
{
  ::core::serialize(out, message.m_type);
  ::core::serialize(out, message.m_clientId);
  ::core::serialize(out, message.m_playerDbId);
  ::core::serialize(out, message.m_resourceDbId);
  ::core::serialize(out, message.m_amount);

  return out;
}

auto operator>>(std::istream &in, LootMessage &message) -> std::istream &
{
  ::core::deserialize(in, message.m_type);
  ::core::deserialize(in, message.m_clientId);
  ::core::deserialize(in, message.m_playerDbId);
  ::core::deserialize(in, message.m_resourceDbId);
  ::core::deserialize(in, message.m_amount);

  return in;
}

} // namespace bsgalone::core
