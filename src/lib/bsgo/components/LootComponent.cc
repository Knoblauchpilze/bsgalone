
#include "LootComponent.hh"

namespace bsgo {

LootComponent::LootComponent(const Uuid &loot, const Item &type)
  : IComponent("loot")
  , m_loot(loot)
  , m_type(type)
{}

auto LootComponent::loot() const -> Uuid
{
  return m_loot;
}

auto LootComponent::type() const -> Item
{
  return m_type;
}

void LootComponent::registerRecipient(const Uuid &entity)
{
  m_recipients.insert(entity);
}

auto LootComponent::recipients() const -> std::unordered_set<Uuid>
{
  return m_recipients;
}

void LootComponent::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
