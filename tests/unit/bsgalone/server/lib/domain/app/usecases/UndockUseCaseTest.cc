
#include "UndockUseCase.hh"
#include "PlayerUndockEvent.hh"
#include "TestGameEventPublisher.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::server {

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_UndockUseCase, ThrowsWhenEventPublisherIsNull)
{
  EXPECT_THROW([]() { UndockUseCase(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Domain_App_Usecases_UndockUseCase,
     PublishesPlayerUndockEventWhenUndockSucceeds)
{
  const core::Uuid playerDbId;

  auto publisher = std::make_shared<TestGameEventPublisher>();
  UndockUseCase usecase(publisher);

  UndockData data{.playerDbId = playerDbId};

  usecase.performUndock(data);

  EXPECT_EQ(1u, publisher->queue().messages().size());
  const auto &event = publisher->queue().messages().at(0);
  EXPECT_EQ(GameEventType::PLAYER_UNDOCK, event->type());
  const auto &actual = event->as<PlayerUndockEvent>();
  EXPECT_EQ(data.playerDbId, actual.getPlayerDbId());
}

} // namespace bsgalone::server
