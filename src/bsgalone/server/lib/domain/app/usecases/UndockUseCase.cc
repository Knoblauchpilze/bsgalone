
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
  auto event = std::make_unique<PlayerUndockEvent>(data.playerDbId);
  // TODO: Should handle undock logic
  m_eventPublisher->publishEvent(std::move(event));
}

} // namespace bsgalone::server
