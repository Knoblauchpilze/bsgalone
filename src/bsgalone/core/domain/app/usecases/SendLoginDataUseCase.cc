
#include "SendLoginDataUseCase.hh"

namespace bsgalone::core {

SendLoginDataUseCase::SendLoginDataUseCase(ForManagingSystemShPtr systemRepo,
                                           ForPublishingPlayerMessageShPtr playerEventBus)
  : m_systemRepo(std::move(systemRepo))
  , m_playerEventBus(std::move(playerEventBus))
{
  if (m_systemRepo == nullptr)
  {
    throw std::invalid_argument("Expected non null system repository");
  }
  if (m_playerEventBus == nullptr)
  {
    throw std::invalid_argument("Expected non null player event bus");
  }
}

void SendLoginDataUseCase::publishLoginData(const Uuid playerDbId)
{
  const auto systems = m_systemRepo->findAll();
  m_playerEventBus->publishSystemList(playerDbId, systems);
}

} // namespace bsgalone::core
