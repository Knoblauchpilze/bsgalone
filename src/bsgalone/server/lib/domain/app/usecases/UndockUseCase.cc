
#include "UndockUseCase.hh"
#include "PlayerUndockEvent.hh"

namespace bsgalone::server {

UndockUseCase::UndockUseCase(ForManagingPlayerShPtr playerRepo,
                             ForPublishingEventShPtr eventPublisher)
  : m_playerRepo(std::move(playerRepo))
  , m_eventPublisher(std::move(eventPublisher))
{
  if (m_playerRepo == nullptr)
  {
    throw std::invalid_argument("Expected non null player repository");
  }
  if (m_eventPublisher == nullptr)
  {
    throw std::invalid_argument("Expected non null event publisher");
  }
}

void UndockUseCase::performUndock(const UndockData &data)
{
  const auto player = m_playerRepo->findOneById(data.playerDbId);
  // TODO: Asteroids should come from the entity manager for the right system
  auto event = std::make_unique<PlayerUndockEvent>(player.dbId, std::vector<core::Asteroid>{});
  m_eventPublisher->publishEvent(std::move(event));
}

} // namespace bsgalone::server
