
#include "InitializeSystemUseCase.hh"

namespace bsgalone::core {

InitializeSystemUseCase::InitializeSystemUseCase(ForManagingAsteroidShPtr asteroidRepo,
                                                 ForRegisteringEntityShPtr entityRegisry)
  : m_asteroidRepo(std::move(asteroidRepo))
  , m_entityRegisry(std::move(entityRegisry))
{
  if (m_asteroidRepo == nullptr)
  {
    throw std::invalid_argument("Expected non null asteroid repository");
  }
  if (m_entityRegisry == nullptr)
  {
    throw std::invalid_argument("Expected non null entity registry");
  }
}

void InitializeSystemUseCase::initializeSystem(const Uuid /*systemDbId*/) {}

} // namespace bsgalone::core
