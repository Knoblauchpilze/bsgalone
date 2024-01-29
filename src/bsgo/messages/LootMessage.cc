
#include "LootMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

LootMessage::LootMessage()
  : NetworkMessage(MessageType::LOOT)
{}

LootMessage::LootMessage(const Uuid resourceId, const float amount)
  : NetworkMessage(MessageType::LOOT)
  , m_resourceId(resourceId)
  , m_amount(amount)
{}

auto LootMessage::resourceId() const -> Uuid
{
  return m_resourceId;
}

auto LootMessage::amount() const -> float
{
  return m_amount;
}

auto LootMessage::serialize(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);
  utils::serialize(out, m_clientId);

  utils::serialize(out, m_resourceId);
  utils::serialize(out, m_amount);

  return out;
}

bool LootMessage::deserialize(std::istream &in)
{
  bool ok{true};
  ok &= utils::deserialize(in, m_messageType);
  ok &= utils::deserialize(in, m_clientId);

  ok &= utils::deserialize(in, m_resourceId);
  ok &= utils::deserialize(in, m_amount);

  return ok;
}

auto LootMessage::clone() const -> IMessagePtr
{
  auto clone = std::make_unique<LootMessage>(m_resourceId, m_amount);
  clone->copyClientIdIfDefined(*this);

  return clone;
}

} // namespace bsgo
