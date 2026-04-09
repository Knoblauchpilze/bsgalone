
#include "OutputUiCommandAdapter.hh"
#include "LoginCommand.hh"
#include "LoginRequest.hh"
#include "LogoutCommand.hh"
#include "LogoutRequest.hh"
#include "MockDataStore.hh"
#include "SignupCommand.hh"
#include "SignupRequest.hh"
#include "TestMessageQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Network_OutputUiCommandAdapter, ThrowsWhenDataStoreIsNull)
{
  EXPECT_THROW([this]() { OutputUiCommandAdapter(nullptr, std::make_shared<TestMessageQueue>()); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Network_OutputUiCommandAdapter, ThrowsWhenQueueIsNull)
{
  EXPECT_THROW([this]() { OutputUiCommandAdapter(std::make_shared<MockDataStore>(), nullptr); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Network_OutputUiCommandAdapter, CorrectlyPublishesLoginRequest)
{
  auto queue = std::make_shared<TestMessageQueue>();
  OutputUiCommandAdapter adapter(std::make_shared<MockDataStore>(), queue);

  LoginCommand command("user", "secure", core::GameRole::GUNNER);
  adapter.onEventReceived(command);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(core::MessageType::LOGIN_REQUEST, queue->messages().at(0)->type());
  const auto &actual = queue->messages().at(0)->as<core::LoginRequest>();
  EXPECT_FALSE(actual.tryGetClientId().has_value());
  EXPECT_EQ("user", actual.getUsername());
  EXPECT_EQ("secure", actual.getPassword());
  EXPECT_EQ(core::GameRole::GUNNER, actual.getRole());
}

TEST(Unit_Bsgalone_Client_Network_OutputUiCommandAdapter, CorrectlyPublishesLogoutRequest)
{
  auto store = std::make_shared<MockDataStore>();
  EXPECT_CALL(*store, getPlayerDbId()).Times(1).WillOnce(Return(core::Uuid{27}));

  auto queue = std::make_shared<TestMessageQueue>();
  OutputUiCommandAdapter adapter(store, queue);

  LogoutCommand command;
  adapter.onEventReceived(command);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(core::MessageType::LOGOUT_REQUEST, queue->messages().at(0)->type());
  const auto &actual = queue->messages().at(0)->as<core::LogoutRequest>();
  EXPECT_EQ(core::Uuid{27}, actual.getPlayerDbId());
}

TEST(Unit_Bsgalone_Client_Network_OutputUiCommandAdapter, CorrectlyPublishesSignupRequest)
{
  auto queue = std::make_shared<TestMessageQueue>();
  OutputUiCommandAdapter adapter(std::make_shared<MockDataStore>(), queue);

  SignupCommand command("user", "secure", core::Faction::CYLON);
  adapter.onEventReceived(command);

  EXPECT_EQ(1u, queue->messages().size());
  EXPECT_EQ(core::MessageType::SIGNUP_REQUEST, queue->messages().at(0)->type());
  const auto &actual = queue->messages().at(0)->as<core::SignupRequest>();
  EXPECT_FALSE(actual.tryGetClientId().has_value());
  EXPECT_EQ("user", actual.getUsername());
  EXPECT_EQ("secure", actual.getPassword());
  EXPECT_EQ(core::Faction::CYLON, actual.getFaction());
}

} // namespace bsgalone::client
