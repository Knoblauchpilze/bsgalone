
#include "LootMessage.hh"

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

} // namespace bsgo
