
#include "NetworkComponent.hh"

namespace bsgo {

namespace {
constexpr auto BASE_SYNC_INTERVAL_MS = 1000;
constexpr auto MAX_SYNC_JITTER_MS    = 100;

auto generateJitteredSyncInterval() -> utils::Duration
{
  const auto jitteredSync = BASE_SYNC_INTERVAL_MS + std::rand() % MAX_SYNC_JITTER_MS;
  return utils::Milliseconds{jitteredSync};
}
} // namespace

NetworkComponent::NetworkComponent(const std::unordered_set<ComponentType> &toSync)
  : AbstractComponent(ComponentType::NETWORK)
  , m_componentsToSync(toSync)
  , m_remainingUntilNextSync(generateJitteredSyncInterval())
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
  m_remainingUntilNextSync = generateJitteredSyncInterval();
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

  m_remainingUntilNextSync -= elapsedMillis;
  if (m_remainingUntilNextSync.count() < 0)
  {
    m_needsSync = true;
  }
}

} // namespace bsgo
