
#include "NetworkEventListener.hh"
#include "ConnectionEstablishedEvent.hh"
#include "ConnectionLostEvent.hh"
#include "LoginMessage.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::client {
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

TEST(Unit_Bsgalone_Client_Network_NetworkEventListener, ThrowsWhenOutputAdapterIsNull)
{
  std::atomic_bool connected;
  EXPECT_THROW([&connected]() { NetworkEventListener(connected, nullptr, {}); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Network_NetworkEventListener,
     ConsidersConnectionEstablishedEventAsRelevant)
{
  std::atomic_bool connected;
  NetworkEventListener listener(connected, std::make_shared<MockOutputAdapter>(), {});

  EXPECT_TRUE(listener.isEventRelevant(net::NetworkEventType::CONNECTION_ESTABLISHED));
}

TEST(Unit_Bsgalone_Client_Network_NetworkEventListener, ConsidersConnectionLostEventAsRelevant)
{
  std::atomic_bool connected;
  NetworkEventListener listener(connected, std::make_shared<MockOutputAdapter>(), {});

  EXPECT_TRUE(listener.isEventRelevant(net::NetworkEventType::CONNECTION_LOST));
}

TEST(Unit_Bsgalone_Client_Network_NetworkEventListener,
     SetsConnectedToTrueWhenServerStartedEventReceived)
{
  std::atomic_bool connected(false);
  NetworkEventListener listener(connected, std::make_shared<MockOutputAdapter>(), {});

  net::ConnectionEstablishedEvent event;
  listener.onEventReceived(event);

  EXPECT_TRUE(connected.load());
}

TEST(Unit_Bsgalone_Client_Network_NetworkEventListener,
     PublishesLoginMessageWhenConnectionIsEstablishedAndAutoLoginIsSet)
{
  std::atomic_bool connected(false);
  auto mockAdapter = std::make_shared<MockOutputAdapter>();
  User autoLogin{
    .name     = "player",
    .password = "secret",
    .role     = core::GameRole::GUNNER,
  };
  NetworkEventListener listener(connected, mockAdapter, autoLogin);

  // https://stackoverflow.com/questions/46157888/gmock-capture-a-reference-argument-in-a-mocked-function
  core::IMessagePtr captured;
  EXPECT_CALL(*mockAdapter, sendMessage(_))
    .Times(1)
    .WillOnce(Invoke([&captured](const core::IMessage &message) { captured = message.clone(); }));

  net::ConnectionEstablishedEvent event;
  listener.onEventReceived(event);

  ASSERT_NE(captured.get(), nullptr) << "Expected a message to be captured";
  EXPECT_EQ(core::MessageType::LOGIN, captured->type());
  const auto &actual = captured->as<core::LoginMessage>();
  EXPECT_EQ("player", actual.getUserName());
  EXPECT_EQ("secret", actual.getPassword());
  EXPECT_EQ(core::GameRole::GUNNER, actual.getGameRole());
  EXPECT_FALSE(actual.tryGetPlayerDbId().has_value());
  EXPECT_FALSE(actual.tryGetSystemDbId().has_value());
}

TEST(Unit_Bsgalone_Client_Network_NetworkEventListener,
     DoesNotPublishLoginMessageWhenAutoLoginIsDisabled)
{
  std::atomic_bool connected(false);
  auto mockAdapter = std::make_shared<MockOutputAdapter>();
  NetworkEventListener listener(connected, mockAdapter, {});

  EXPECT_CALL(*mockAdapter, sendMessage(_)).Times(0);

  net::ConnectionEstablishedEvent event;
  listener.onEventReceived(event);
}

TEST(Unit_Bsgalone_Client_Network_NetworkEventListener,
     SetsConnectedToFalseWhenServerStoppedEventReceived)
{
  std::atomic_bool connected(true);
  NetworkEventListener listener(connected, std::make_shared<MockOutputAdapter>(), {});

  net::ConnectionLostEvent event;
  listener.onEventReceived(event);

  EXPECT_FALSE(connected.load());
}

} // namespace bsgalone::client
