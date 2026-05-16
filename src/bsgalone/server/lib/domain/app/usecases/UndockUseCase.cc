
#include "UndockUseCase.hh"
#include "PlayerUndockEvent.hh"

namespace bsgalone::server {

UndockUseCase::UndockUseCase(ForPublishingEventShPtr eventPublisher)
  : m_eventPublisher(std::move(eventPublisher))
{
  if (m_eventPublisher == nullptr)
  {
    throw std::invalid_argument("Expected non null event publisher");
  }
}

void UndockUseCase::performUndock(const UndockData &data)
{
  // TODO: Asteroids should come from the entity manager for the right system
  auto event = std::make_unique<PlayerUndockEvent>(data.playerDbId, std::vector<core::Asteroid>{});
  m_eventPublisher->publishEvent(std::move(event));
}

} // namespace bsgalone::server
