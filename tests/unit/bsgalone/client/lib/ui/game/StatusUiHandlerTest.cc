
#include "StatusUiHandler.hh"
#include "LogoutCommand.hh"
#include "MockTexturePack.hh"
#include "TestUiCommandQueue.hh"
#include "TestUiEventQueue.hh"
#include "TestUiInputFactory.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {
namespace {
class Unit_Bsgalone_Client_Ui_Game_StatusUiHandler : public Test
{
  protected:
  void SetUp() override
  {
    uiCommandQueue = std::make_shared<TestUiCommandQueue>(false);
    handler        = std::make_unique<StatusUiHandler>(std::make_shared<TestUiEventQueue>(false),
                                                uiCommandQueue);
  }

  void TearDown() override
  {
    // Intentionally empty
  }

  std::shared_ptr<TestUiCommandQueue> uiCommandQueue{};

  std::unique_ptr<StatusUiHandler> handler{};
};
} // namespace

TEST_F(Unit_Bsgalone_Client_Ui_Game_StatusUiHandler, ThrowsWhenCommandQueueIsNull)
{
  EXPECT_THROW([this]() { StatusUiHandler(std::make_shared<TestUiEventQueue>(false), nullptr); }(),
               std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Client_Ui_Game_StatusUiHandler, ThrowsWhenEventQueueIsNull)
{
  EXPECT_THROW([this]() { StatusUiHandler(nullptr, this->uiCommandQueue); }(),
               std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Client_Ui_Game_StatusUiHandler, PublishesLogoutCommandWhenLogoutIsConfirmed)
{
  MockTexturePack pack;
  handler->initializeMenus(pge::Vec2i{1000, 1000}, pack);

  // First click on the logout panel
  auto data      = generateInputForClickAt(pge::Vec2i{799, 24});
  auto processed = handler->processUserInput(data);

  // Update the UI to make the menu visible
  handler->updateUi();

  // Then confirm the logout
  data      = generateInputForClickAt(pge::Vec2i{415, 559});
  processed = handler->processUserInput(data);

  EXPECT_TRUE(processed);
  EXPECT_EQ(1u, uiCommandQueue->messages().size());
  EXPECT_EQ(UiCommandType::LOGOUT_REQUESTED, uiCommandQueue->messages().at(0)->type());
}

TEST_F(Unit_Bsgalone_Client_Ui_Game_StatusUiHandler,
       DoesNotPublishLogoutCommandWhenLogoutIsCancelled)
{
  MockTexturePack pack;
  handler->initializeMenus(pge::Vec2i{1000, 1000}, pack);

  // First click on the logout panel
  auto data      = generateInputForClickAt(pge::Vec2i{988, 6});
  auto processed = handler->processUserInput(data);

  // Update the UI to make the menu visible
  handler->updateUi();

  // Then cancel the logout
  data      = generateInputForClickAt(pge::Vec2i{597, 500});
  processed = handler->processUserInput(data);

  EXPECT_TRUE(processed);
  EXPECT_TRUE(uiCommandQueue->empty());
}

} // namespace bsgalone::client
