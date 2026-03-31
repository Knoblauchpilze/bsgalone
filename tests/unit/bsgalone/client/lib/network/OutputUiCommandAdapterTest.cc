
#include "OutputUiCommandAdapter.hh"
#include "LoginCommand.hh"
#include "LoginRequest.hh"
#include "SignupCommand.hh"
#include "SignupRequest.hh"
#include "TestMessageQueue.hh"
#include <gtest/gtest.h>

using namespace test;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Network_OutputUiCommandAdapter, ThrowsWhenQueueIsNull)
{
  EXPECT_THROW([this]() { OutputUiCommandAdapter(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Client_Network_OutputUiCommandAdapter, CorrectlyPublishesLoginRequest)
{
  auto queue = std::make_shared<TestMessageQueue>();
  OutputUiCommandAdapter adapter(queue);

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

TEST(Unit_Bsgalone_Client_Network_OutputUiCommandAdapter, CorrectlyPublishesSignupRequest)
{
  auto queue = std::make_shared<TestMessageQueue>();
  OutputUiCommandAdapter adapter(queue);

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
