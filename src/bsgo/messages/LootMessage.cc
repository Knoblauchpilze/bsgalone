
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

auto LootMessage::operator<<(std::ostream &out) const -> std::ostream &
{
  utils::serialize(out, m_messageType);

  out << m_resourceId;
  out << m_amount;

  return out;
}

auto LootMessage::operator>>(std::istream &in) -> std::istream &
{
  utils::deserialize(in, m_messageType);

  in >> m_resourceId;
  in >> m_amount;

  return in;
}

} // namespace bsgo
