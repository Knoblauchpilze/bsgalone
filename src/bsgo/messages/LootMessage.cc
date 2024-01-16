
#include "LootMessage.hh"
#include <core_utils/SerializationUtils.hh>

namespace bsgo {

LootMessage::LootMessage(const Uuid &resourceId, const float amount)
  : AbstractMessage(MessageType::LOOT)
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

  utils::serialize(out, m_resourceId);
  utils::serialize(out, m_amount);

  return out;
}

auto LootMessage::deserialize(std::istream &in) -> std::istream &
{
  utils::deserialize(in, m_messageType);

  utils::deserialize(in, m_resourceId);
  utils::deserialize(in, m_amount);

  return in;
}

} // namespace bsgo
