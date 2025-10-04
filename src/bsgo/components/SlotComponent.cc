
#include "SlotComponent.hh"

namespace bsgo {

SlotComponent::SlotComponent(const ComponentType &type, const SlotComponentData &data)
  : AbstractComponent(type)
  , m_dbId(data.dbId)
  , m_offensive(data.offensive)
  , m_powerCost(data.powerCost)
  , m_range(data.range)
  , m_reloadTime(data.reloadTime)
{
  addModule("slot");
}

void SlotComponent::update(const float elapsedSeconds)
{
  handleReload(elapsedSeconds);
}

auto SlotComponent::dbId() const -> Uuid
{
  return m_dbId;
}

bool SlotComponent::isOffensive() const
{
  return m_offensive;
}

auto SlotComponent::powerCost() const -> float
{
  return m_powerCost;
}

auto SlotComponent::maybeRange() const -> std::optional<float>
{
  return m_range;
}

auto SlotComponent::range() const -> float
{
  if (!m_range)
  {
    error("Expected component to have a range");
  }
  return *m_range;
}

auto SlotComponent::firingState() const noexcept -> FiringState
{
  return m_firingState;
}

bool SlotComponent::canFire() const noexcept
{
  return FiringState::READY == firingState();
}

bool SlotComponent::isReloading() const noexcept
{
  return m_elapsedSinceLastFired.has_value();
}

auto SlotComponent::reloadPercentage() const -> float
{
  if (!isReloading())
  {
    return 1.0f;
  }

  // TODO: We should not convert to milliseconds here.
  const auto reloadTime = core::toMilliseconds(m_reloadTime.count());

  // https://stackoverflow.com/questions/76522118/dividing-two-chronodurations-to-get-fraction
  const auto reloadAsFloat = std::chrono::duration<float, std::milli>(reloadTime);
  return *m_elapsedSinceLastFired / reloadAsFloat;
}

auto SlotComponent::elapsedSinceLastFired() const -> std::optional<core::Duration>
{
  return m_elapsedSinceLastFired;
}

void SlotComponent::overrideElapsedSinceLastFired(const std::optional<core::Duration> &elapsed)
{
  m_elapsedSinceLastFired = elapsed;
}

void SlotComponent::setFiringState(const FiringState &firingState)
{
  m_firingState = firingState;
}

void SlotComponent::registerFireRequest()
{
  m_fireRequest = true;
}

bool SlotComponent::hasFireRequest() const
{
  return m_fireRequest;
}

void SlotComponent::fire()
{
  clearFireRequest();
  if (!canFire())
  {
    error("Failed to use slot", "Still reloading");
  }

  m_elapsedSinceLastFired = core::Duration(0);
}

void SlotComponent::clearFireRequest()
{
  m_fireRequest = false;
}

void SlotComponent::handleReload(const float elapsedSeconds)
{
  if (!m_elapsedSinceLastFired)
  {
    return;
  }

  constexpr auto MILLISECONDS_IN_A_SECONDS = 1000;
  (*m_elapsedSinceLastFired) += core::Milliseconds(
    static_cast<int>(elapsedSeconds * MILLISECONDS_IN_A_SECONDS));

  // TODO: We should not convert to milliseconds here.
  const auto reloadTime = core::toMilliseconds(m_reloadTime.count());

  if (*m_elapsedSinceLastFired >= reloadTime)
  {
    m_elapsedSinceLastFired.reset();
  }
}

} // namespace bsgo
