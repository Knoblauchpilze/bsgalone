
#include "OutputGameEventAdapter.hh"
#include "ClientManager.hh"
#include "LoginMessage.hh"
#include "PlayerLoginEvent.hh"
#include "PlayerSignupEvent.hh"
#include "SignupMessage.hh"
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

TEST(Unit_Bsgalone_Server_Events_OutputGameEventAdapter, ThrowsWhenClientManagerIsNull)
{
  EXPECT_THROW([]() { OutputGameEventAdapter(nullptr, std::make_unique<MockOutputAdapter>()); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Events_OutputGameEventAdapter, ThrowsWhenNetworkClientIsNull)
{
  EXPECT_THROW([]() { OutputGameEventAdapter(std::make_shared<ClientManager>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Server_Events_OutputGameEventAdapter, ForwardsFailedLoginMessage)
{
  PlayerLoginEvent event(net::ClientId{12});

  auto networkClient = std::make_unique<StrictMock<MockOutputAdapter>>();
  core::IMessagePtr captured;
  EXPECT_CALL(*networkClient, sendMessage(net::ClientId{12}, _))
    .Times(1)
    .WillOnce(Invoke([&captured](const net::ClientId /*clientId*/, const core::IMessage &message) {
      captured = message.clone();
    }));

  OutputGameEventAdapter adapter(std::make_shared<ClientManager>(), std::move(networkClient));
  adapter.onEventReceived(event);

  EXPECT_EQ(core::MessageType::LOGIN, captured->type());
  const auto &actual = captured->as<core::LoginMessage>();
  EXPECT_FALSE(actual.successfullyLoggedIn());
}

TEST(Unit_Bsgalone_Server_Events_OutputGameEventAdapter, ForwardsSuccessfulLoginMessage)
{
  PlayerLoginEvent event(net::ClientId{12});
  event.setPlayerDbId(core::Uuid{18});
  event.setRole(core::GameRole::GUNNER);

  auto networkClient = std::make_unique<StrictMock<MockOutputAdapter>>();
  core::IMessagePtr captured;
  EXPECT_CALL(*networkClient, sendMessage(net::ClientId{12}, _))
    .Times(1)
    .WillOnce(Invoke([&captured](const net::ClientId /*clientId*/, const core::IMessage &message) {
      captured = message.clone();
    }));

  OutputGameEventAdapter adapter(std::make_shared<ClientManager>(), std::move(networkClient));
  adapter.onEventReceived(event);

  EXPECT_EQ(core::MessageType::LOGIN, captured->type());
  const auto &actual = captured->as<core::LoginMessage>();
  EXPECT_TRUE(actual.successfullyLoggedIn());
  EXPECT_EQ(core::Uuid{18}, actual.getPlayerDbId());
  EXPECT_EQ(core::GameRole::GUNNER, actual.getRole());
}

TEST(Unit_Bsgalone_Server_Events_OutputGameEventAdapter, ForwardsFailedSignupMessage)
{
  PlayerSignupEvent event(net::ClientId{12});

  auto networkClient = std::make_unique<StrictMock<MockOutputAdapter>>();
  core::IMessagePtr captured;
  EXPECT_CALL(*networkClient, sendMessage(net::ClientId{12}, _))
    .Times(1)
    .WillOnce(Invoke([&captured](const net::ClientId /*clientId*/, const core::IMessage &message) {
      captured = message.clone();
    }));

  OutputGameEventAdapter adapter(std::make_shared<ClientManager>(), std::move(networkClient));
  adapter.onEventReceived(event);

  EXPECT_EQ(core::MessageType::SIGNUP, captured->type());
  const auto &actual = captured->as<core::SignupMessage>();
  EXPECT_FALSE(actual.successfullySignedUp());
}

TEST(Unit_Bsgalone_Server_Events_OutputGameEventAdapter, ForwardsSuccessfulSignupMessage)
{
  PlayerSignupEvent event(net::ClientId{12});
  event.setPlayerDbId(core::Uuid{18});
  event.setFaction(core::Faction::CYLON);

  auto networkClient = std::make_unique<StrictMock<MockOutputAdapter>>();
  core::IMessagePtr captured;
  EXPECT_CALL(*networkClient, sendMessage(net::ClientId{12}, _))
    .Times(1)
    .WillOnce(Invoke([&captured](const net::ClientId /*clientId*/, const core::IMessage &message) {
      captured = message.clone();
    }));

  OutputGameEventAdapter adapter(std::make_shared<ClientManager>(), std::move(networkClient));
  adapter.onEventReceived(event);

  EXPECT_EQ(core::MessageType::SIGNUP, captured->type());
  const auto &actual = captured->as<core::SignupMessage>();
  EXPECT_TRUE(actual.successfullySignedUp());
}

} // namespace bsgalone::server
