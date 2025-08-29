
#include "ResourceComponent.hh"

namespace bsgo {

ResourceComponent::ResourceComponent(const Uuid resource, const int amount)
  : AbstractComponent(ComponentType::RESOURCE)
  , m_resource(resource)
  , m_amount(amount)
{}

auto ResourceComponent::resource() const -> Uuid
{
  return m_resource;
}

auto ResourceComponent::amount() const -> int
{
  return m_amount;
}

void ResourceComponent::update(const float /*elapsedSeconds*/) {}

void ResourceComponent::setAmount(const int amount)
{
  if (amount < 0)
  {
    error("Failed to set amount of " + str(m_resource) + " to " + std::to_string(amount),
          "Expected positive amount");
  }
  m_amount = amount;
}

} // namespace bsgo
