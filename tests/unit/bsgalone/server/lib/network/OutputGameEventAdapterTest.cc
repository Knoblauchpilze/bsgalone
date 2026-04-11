
#include "OutputGameEventAdapter.hh"
#include "ClientManager.hh"
#include "LoginMessage.hh"
#include "LogoutMessage.hh"
#include "PlayerLoginEvent.hh"
#include "PlayerLogoutEvent.hh"
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

void expectSendMessageAndCaptureArg(MockOutputAdapter &adapter, core::IMessagePtr &captured)
{
  EXPECT_CALL(adapter, sendMessage(net::ClientId{12}, _))
    .Times(1)
    .WillOnce(Invoke([&captured](const net::ClientId /*clientId*/, const core::IMessage &message) {
      captured = message.clone();
    }));
}
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
  expectSendMessageAndCaptureArg(*networkClient, captured);

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
  expectSendMessageAndCaptureArg(*networkClient, captured);

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
  expectSendMessageAndCaptureArg(*networkClient, captured);

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
  expectSendMessageAndCaptureArg(*networkClient, captured);

  OutputGameEventAdapter adapter(std::make_shared<ClientManager>(), std::move(networkClient));
  adapter.onEventReceived(event);

  EXPECT_EQ(core::MessageType::SIGNUP, captured->type());
  const auto &actual = captured->as<core::SignupMessage>();
  EXPECT_TRUE(actual.successfullySignedUp());
}

TEST(Unit_Bsgalone_Server_Events_OutputGameEventAdapter, ForwardsLogoutMessageToAllRegisteredClients)
{
  PlayerLogoutEvent event(core::Uuid{18});
  auto clientManager = std::make_shared<ClientManager>();
  clientManager->registerClient(net::ClientId{12});
  clientManager->registerPlayer(net::ClientId{12}, core::Uuid{18}, core::Uuid{19});

  clientManager->registerClient(net::ClientId{13});

  auto networkClient = std::make_unique<StrictMock<MockOutputAdapter>>();
  core::IMessagePtr captured1;
  EXPECT_CALL(*networkClient, sendMessage(net::ClientId{12}, _))
    .Times(1)
    .WillOnce(Invoke([&captured1](const net::ClientId /*clientId*/, const core::IMessage &message) {
      captured1 = message.clone();
    }));
  core::IMessagePtr captured2;
  EXPECT_CALL(*networkClient, sendMessage(net::ClientId{13}, _))
    .Times(1)
    .WillOnce(Invoke([&captured2](const net::ClientId /*clientId*/, const core::IMessage &message) {
      captured2 = message.clone();
    }));

  OutputGameEventAdapter adapter(clientManager, std::move(networkClient));
  adapter.onEventReceived(event);

  EXPECT_EQ(core::MessageType::LOGOUT, captured1->type());
  EXPECT_EQ(core::Uuid{18}, captured1->as<core::LogoutMessage>().getPlayerDbId());

  EXPECT_EQ(core::MessageType::LOGOUT, captured2->type());
  EXPECT_EQ(core::Uuid{18}, captured2->as<core::LogoutMessage>().getPlayerDbId());
}

} // namespace bsgalone::server
