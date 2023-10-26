
#include "LootComponent.hh"

namespace bsgo {

LootComponent::LootComponent(const float amount)
  : IComponent("loot")
  , m_amount(amount)
{
  if (m_amount <= 0.0f)
  {
    throw std::invalid_argument("Expected positive amount got " + std::to_string(amount));
  }
}

auto LootComponent::amount() const -> float
{
  return m_amount;
}

void LootComponent::registerRecipient(const Uuid &entity)
{
  m_recipients.insert(entity);
}

void LootComponent::update(const float /*elapsedSeconds*/) {}

} // namespace bsgo
