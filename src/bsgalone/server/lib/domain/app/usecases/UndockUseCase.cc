
#include "UndockUseCase.hh"
#include "PlayerUndockEvent.hh"

namespace bsgalone::server {

UndockUseCase::UndockUseCase(ForManagingPlayerShPtr playerRepo,
                             ForManagingSystemShPtr systemRepo,
                             SystemsManagerShPtr systemsManager,
                             ForPublishingEventShPtr eventPublisher)
  : m_playerRepo(std::move(playerRepo))
  , m_systemRepo(std::move(systemRepo))
  , m_systemsManager(std::move(systemsManager))
  , m_eventPublisher(std::move(eventPublisher))
{
  if (m_playerRepo == nullptr)
  {
    throw std::invalid_argument("Expected non null player repository");
  }
  if (m_systemRepo == nullptr)
  {
    throw std::invalid_argument("Expected non null system repository");
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
  const auto player = m_playerRepo->findOneById(data.playerDbId);
  // TODO: Use the system in the event
  /*const auto system  = */ m_systemRepo->findOneById(player.systemDbId);
  const auto manager = m_systemsManager->entityManagerFor(player.systemDbId);
  auto event         = std::make_unique<PlayerUndockEvent>(player.dbId, manager->getAsteroids());
  m_eventPublisher->publishEvent(std::move(event));
}

} // namespace bsgalone::server
