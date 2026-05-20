
#include "UndockUseCase.hh"
#include "EntityRegistryFixture.hh"
#include "MockPlayerRepository.hh"
#include "MockSystemRepository.hh"
#include "PlayerUndockEvent.hh"
#include "SystemsManager.hh"
#include "TestDataFactory.hh"
#include "TestGameEventPublisher.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {

using Unit_Bsgalone_Server_Domain_App_Usecases_UndockUseCase = EntityRegistryFixture;

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_UndockUseCase, ThrowsWhenPlayerRepositoryIsNull)
{
  EXPECT_THROW(
    []() {
      UndockUseCase(nullptr,
                    std::make_shared<MockSystemRepository>(),
                    std::make_shared<SystemsManager>(),
                    std::make_shared<TestGameEventPublisher>());
    }(),
    std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_UndockUseCase, ThrowsWhenSystemRepositoryIsNull)
{
  EXPECT_THROW(
    []() {
      UndockUseCase(std::make_shared<MockPlayerRepository>(),
                    nullptr,
                    std::make_shared<SystemsManager>(),
                    std::make_shared<TestGameEventPublisher>());
    }(),
    std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_UndockUseCase, ThrowsWhenSystemsManagerIsNull)
{
  EXPECT_THROW(
    []() {
      UndockUseCase(std::make_shared<MockPlayerRepository>(),
                    std::make_shared<MockSystemRepository>(),
                    nullptr,
                    std::make_shared<TestGameEventPublisher>());
    }(),
    std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_UndockUseCase, ThrowsWhenEventPublisherIsNull)
{
  EXPECT_THROW(
    []() {
      UndockUseCase(std::make_shared<MockPlayerRepository>(),
                    std::make_shared<MockSystemRepository>(),
                    std::make_shared<SystemsManager>(),
                    nullptr);
    }(),
    std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_UndockUseCase,
       PublishesPlayerUndockEventWhenUndockSucceeds)
{
  const auto player   = generatePlayer({});
  auto mockPlayerRepo = std::make_shared<StrictMock<MockPlayerRepository>>();
  EXPECT_CALL(*mockPlayerRepo, findOneById(player.dbId)).Times(1).WillOnce(Return(player));

  const auto system   = generateSystem(player.systemDbId);
  auto mockSystemRepo = std::make_shared<StrictMock<MockSystemRepository>>();
  EXPECT_CALL(*mockSystemRepo, findOneById(player.systemDbId)).Times(1).WillOnce(Return(system));

  core::Asteroid asteroid1 = generateAsteroid();
  this->registerAsteroid(asteroid1);
  core::Asteroid asteroid2 = generateAsteroid();
  this->registerAsteroid(asteroid2);

  auto manager = std::make_shared<SystemsManager>();
  manager->registerSystem(player.systemDbId, this->entityRegistry());

  auto publisher = std::make_shared<TestGameEventPublisher>();
  UndockUseCase usecase(mockPlayerRepo, mockSystemRepo, std::move(manager), publisher);

  UndockData data{.playerDbId = player.dbId};

  usecase.performUndock(data);

  EXPECT_EQ(1u, publisher->queue().messages().size());
  const auto &event = publisher->queue().messages().at(0);
  EXPECT_EQ(GameEventType::PLAYER_UNDOCK, event->type());
  const auto &actual = event->as<PlayerUndockEvent>();
  EXPECT_EQ(player.dbId, actual.getPlayerDbId());
  EXPECT_EQ(system, actual.getSystem());
  std::vector<core::Asteroid> expectedAsteroids{asteroid2, asteroid1};
  EXPECT_EQ(expectedAsteroids, actual.getAsteroids());
}

TEST_F(Unit_Bsgalone_Server_Domain_App_Usecases_UndockUseCase, FailsWhenPlayerIsInUnknownSystem)
{
  const auto player = generatePlayer({});

  auto mockPlayerRepo = std::make_shared<StrictMock<MockPlayerRepository>>();
  EXPECT_CALL(*mockPlayerRepo, findOneById(player.dbId)).Times(1).WillOnce(Return(player));

  const auto system   = generateSystem(player.systemDbId);
  auto mockSystemRepo = std::make_shared<StrictMock<MockSystemRepository>>();
  EXPECT_CALL(*mockSystemRepo, findOneById(player.systemDbId)).Times(1).WillOnce(Return(system));

  auto manager   = std::make_shared<SystemsManager>();
  auto publisher = std::make_shared<TestGameEventPublisher>();
  UndockUseCase usecase(mockPlayerRepo, mockSystemRepo, std::move(manager), publisher);

  UndockData data{.playerDbId = player.dbId};

  auto code = [&data, &usecase]() { usecase.performUndock(data); };
  EXPECT_THROW(code(), std::invalid_argument);

  EXPECT_TRUE(publisher->queue().messages().empty());
}

} // namespace bsgalone::server
