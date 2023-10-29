
#include "LootComponent.hh"

namespace bsgo {

LootComponent::LootComponent()
  : IComponent("loot")
{}

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
