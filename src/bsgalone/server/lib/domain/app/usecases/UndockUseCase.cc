
#include "UndockUseCase.hh"
#include "PlayerUndockEvent.hh"

namespace bsgalone::server {

UndockUseCase::UndockUseCase(ForManagingPlayerShPtr playerRepo,
                             SystemsManagerShPtr systemsManager,
                             ForPublishingEventShPtr eventPublisher)
  : m_playerRepo(std::move(playerRepo))
  , m_systemsManager(std::move(systemsManager))
  , m_eventPublisher(std::move(eventPublisher))
{
  if (m_playerRepo == nullptr)
  {
    throw std::invalid_argument("Expected non null player repository");
  }
  if (m_systemsManager == nullptr)
  {
    throw std::invalid_argument("Expected non null systems manager");
  }
  if (m_eventPublisher == nullptr)
  {
    throw std::invalid_argument("Expected non null event publisher");
  }
}

void UndockUseCase::performUndock(const UndockData &data)
{
  const auto player  = m_playerRepo->findOneById(data.playerDbId);
  const auto manager = m_systemsManager->entityManagerFor(player.systemDbId);
  auto event         = std::make_unique<PlayerUndockEvent>(player.dbId, manager->getAsteroids());
  m_eventPublisher->publishEvent(std::move(event));
}

} // namespace bsgalone::server
