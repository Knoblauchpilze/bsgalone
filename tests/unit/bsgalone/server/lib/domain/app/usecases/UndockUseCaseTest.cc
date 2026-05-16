
#include "UndockUseCase.hh"
#include "MockPlayerRepository.hh"
#include "PlayerUndockEvent.hh"
#include "TestDataFactory.hh"
#include "TestGameEventPublisher.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_UndockUseCase, ThrowsWhenPlayerRepositoryIsNull)
{
  EXPECT_THROW([]() { UndockUseCase(nullptr, std::make_shared<TestGameEventPublisher>()); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_UndockUseCase, ThrowsWhenEventPublisherIsNull)
{
  EXPECT_THROW([]() { UndockUseCase(std::make_shared<MockPlayerRepository>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_UndockUseCase,
     PublishesPlayerUndockEventWhenUndockSucceeds)
{
  const auto player = generatePlayer({});

  auto mockPlayerRepo = std::make_shared<StrictMock<MockPlayerRepository>>();
  EXPECT_CALL(*mockPlayerRepo, findOneById(player.dbId)).Times(1).WillOnce(Return(player));

  auto publisher = std::make_shared<TestGameEventPublisher>();
  UndockUseCase usecase(mockPlayerRepo, publisher);

  UndockData data{.playerDbId = player.dbId};

  usecase.performUndock(data);

  EXPECT_EQ(1u, publisher->queue().messages().size());
  const auto &event = publisher->queue().messages().at(0);
  EXPECT_EQ(GameEventType::PLAYER_UNDOCK, event->type());
  const auto &actual = event->as<PlayerUndockEvent>();
  EXPECT_EQ(player.dbId, actual.getPlayerDbId());
  // TODO: Should verify that the asteroids come from the right system
  EXPECT_EQ(std::vector<core::Asteroid>{}, actual.getAsteroids());
}

} // namespace bsgalone::server
