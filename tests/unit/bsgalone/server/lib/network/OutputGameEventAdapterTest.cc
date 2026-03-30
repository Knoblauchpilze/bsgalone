
#include "OutputGameEventAdapter.hh"
#include "LoginMessage.hh"
#include "PlayerLoginEvent.hh"
#include "TestMessageQueue.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::server {
namespace {
class MockOutputAdapter : public core::IOutputNetworkAdapter
{
  public:
  MockOutputAdapter()           = default;
  ~MockOutputAdapter() override = default;

  MOCK_METHOD(void, sendMessage, (const net::ClientId, const core::IMessage &), (override));
  MOCK_METHOD(void, sendMessage, (const core::IMessage &), (override));
};
} // namespace

TEST(Unit_Bsgalone_Server_Events_OutputGameEventAdapter, ThrowsWhenNetworkClientIsNull)
{
  EXPECT_THROW([]() { OutputGameEventAdapter(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Events_OutputGameEventAdapter, ForwardsFailedLoginMessage)
{
  core::PlayerLoginEvent event(net::ClientId{12});

  auto networkClient = std::make_unique<StrictMock<MockOutputAdapter>>();
  core::IMessagePtr captured;
  EXPECT_CALL(*networkClient, sendMessage(net::ClientId{12}, _))
    .Times(1)
    .WillOnce(Invoke([&captured](const net::ClientId /*clientId*/, const core::IMessage &message) {
      captured = message.clone();
    }));

  OutputGameEventAdapter adapter(std::move(networkClient));
  adapter.onEventReceived(event);

  EXPECT_EQ(core::MessageType::LOGIN, captured->type());
  const auto &actual = captured->as<core::LoginMessage>();
  EXPECT_EQ(net::ClientId{12}, actual.getClientId());
  EXPECT_FALSE(actual.successfullyLoggedIn());
}

TEST(Unit_Bsgalone_Server_Events_OutputGameEventAdapter, ForwardsSuccessfulLoginMessage)
{
  core::PlayerLoginEvent event(net::ClientId{12});
  event.setPlayerDbId(core::Uuid{18});
  event.setRole(core::GameRole::GUNNER);
  event.setSystemDbId(core::Uuid{19});

  auto networkClient = std::make_unique<StrictMock<MockOutputAdapter>>();
  core::IMessagePtr captured;
  EXPECT_CALL(*networkClient, sendMessage(net::ClientId{12}, _))
    .Times(1)
    .WillOnce(Invoke([&captured](const net::ClientId /*clientId*/, const core::IMessage &message) {
      captured = message.clone();
    }));

  OutputGameEventAdapter adapter(std::move(networkClient));
  adapter.onEventReceived(event);

  EXPECT_EQ(core::MessageType::LOGIN, captured->type());
  const auto &actual = captured->as<core::LoginMessage>();
  EXPECT_EQ(net::ClientId{12}, actual.getClientId());
  EXPECT_TRUE(actual.successfullyLoggedIn());
  EXPECT_EQ(core::Uuid{18}, actual.getPlayerDbId());
  EXPECT_EQ(core::GameRole::GUNNER, actual.getRole());
  EXPECT_EQ(core::Uuid{19}, actual.getSystemDbId());
}

} // namespace bsgalone::server
