
#include "NetworkComponent.hh"

namespace bsgo {

NetworkComponent::NetworkComponent(const std::unordered_set<ComponentType> &toSync)
  : AbstractComponent(ComponentType::NETWORK)
  , m_componentsToSync(toSync)
{}

bool NetworkComponent::needsSync() const
{
  return m_needsSync;
}

void NetworkComponent::markForSync(const bool needsSync)
{
  m_needsSync = needsSync;
}

void NetworkComponent::markAsJustSynced()
{
  markForSync(false);
  m_elapsedSinceLastSync = utils::Duration{0};
}

auto NetworkComponent::getElapsedSinceLastSync() const -> utils::Duration
{
  return m_elapsedSinceLastSync;
}

auto NetworkComponent::componentsToSync() const -> const std::unordered_set<ComponentType> &
{
  return m_componentsToSync;
}

void NetworkComponent::update(const float elapsedSeconds)
{
  constexpr auto MILLISECONDS_IN_A_SECONDS = 1000;
  const auto elapsedMillis                 = utils::Milliseconds(
    static_cast<int>(elapsedSeconds * MILLISECONDS_IN_A_SECONDS));

  m_elapsedSinceLastSync += elapsedMillis;
}

} // namespace bsgo
