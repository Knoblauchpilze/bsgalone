
#include "SlotComponent.hh"

namespace bsgo {

SlotComponent::SlotComponent(const ComponentType &type, const SlotComponentData &data)
  : AbstractComponent(type)
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

  // https://stackoverflow.com/questions/76522118/dividing-two-chronodurations-to-get-fraction
  const auto reloadAsFloat = std::chrono::duration<float, std::milli>(m_reloadTime);
  return *m_elapsedSinceLastFired / reloadAsFloat;
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

  m_elapsedSinceLastFired = utils::Duration(0);
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
  (*m_elapsedSinceLastFired) += utils::Milliseconds(
    static_cast<int>(elapsedSeconds * MILLISECONDS_IN_A_SECONDS));

  if (*m_elapsedSinceLastFired >= m_reloadTime)
  {
    m_elapsedSinceLastFired.reset();
  }
}

} // namespace bsgo
