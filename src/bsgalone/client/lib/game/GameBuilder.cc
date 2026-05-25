
#include "GameBuilder.hh"
#include "Game.hh"

namespace bsgalone::client {

struct GameBuilder::ptrEnabler : public Game
{
  ptrEnabler()
    : Game() {};
};

auto GameBuilder::build() -> IGameShPtr
{
  validate();

  auto game = std::make_shared<ptrEnabler>();

  game->m_entityRegistry  = m_entityRegistry;
  game->m_coordinator     = std::move(m_coordinator);
  game->m_asteroidCreator = std::move(m_asteroidCreator);

  return game;
}

auto GameBuilder::withEntityRegistry(core::EntityRegistryShPtr entityRegistry) -> GameBuilder &
{
  m_entityRegistry = std::move(entityRegistry);
  return *this;
}

auto GameBuilder::withSimulationRunner(core::ForRunningSimulationPtr coordinator) -> GameBuilder &
{
  m_coordinator = std::move(coordinator);
  return *this;
}

auto GameBuilder::withAsteroidCreator(core::ForCreatingAsteroidPtr creator) -> GameBuilder &
{
  m_asteroidCreator = std::move(creator);
  return *this;
}

void GameBuilder::validate() const
{
  if (m_entityRegistry == nullptr)
  {
    throw std::invalid_argument("Expected value for entity registry");
  }
  if (m_coordinator == nullptr)
  {
    throw std::invalid_argument("Expected value for coordinator");
  }
  if (m_asteroidCreator == nullptr)
  {
    throw std::invalid_argument("Expected value for asteroid creator");
  }
}

} // namespace bsgalone::client
