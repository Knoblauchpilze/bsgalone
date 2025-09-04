
#include "SyncComponent.hh"

namespace bsgo {

namespace {
constexpr auto BASE_SYNC_INTERVAL_MS = 1000;
constexpr auto MAX_SYNC_JITTER_MS    = 100;

auto generateJitteredSyncInterval() -> core::Duration
{
  const auto jitteredSync = BASE_SYNC_INTERVAL_MS + std::rand() % MAX_SYNC_JITTER_MS;
  return core::Milliseconds{jitteredSync};
}
} // namespace

SyncComponent::SyncComponent(const ComponentType type)
  : AbstractComponent(type)
  , m_remainingUntilNextSync(generateJitteredSyncInterval())
{}

bool SyncComponent::needsSync() const
{
  return m_needsSync;
}

void SyncComponent::markForSync(const bool needsSync)
{
  m_needsSync = needsSync;
}

void SyncComponent::markAsJustSynced()
{
  markForSync(false);
  m_remainingUntilNextSync = generateJitteredSyncInterval();
}

void SyncComponent::update(const float elapsedSeconds)
{
  constexpr auto MILLISECONDS_IN_A_SECONDS = 1000;
  const auto elapsedMillis                 = core::Milliseconds(
    static_cast<int>(elapsedSeconds * MILLISECONDS_IN_A_SECONDS));

  m_remainingUntilNextSync -= elapsedMillis;
  if (m_remainingUntilNextSync.count() < 0)
  {
    m_needsSync = true;
  }
}

} // namespace bsgo
