
#include "OutpostUiHandler.hh"
#include "MockDataStore.hh"
#include "MockTexturePack.hh"
#include "TestUiCommandQueue.hh"
#include "TestUiEventQueue.hh"
#include "TestUiInputFactory.hh"
#include "UndockCommand.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {
namespace {
class Unit_Bsgalone_Client_Ui_Outpost_OutpostUiHandler : public Test
{
  protected:
  void SetUp() override
  {
    mockDataStore  = std::make_shared<StrictMock<MockDataStore>>();
    uiEventQueue   = std::make_shared<TestUiEventQueue>(false);
    uiCommandQueue = std::make_shared<TestUiCommandQueue>(false);
    handler = std::make_unique<OutpostUiHandler>(mockDataStore, uiEventQueue, uiCommandQueue);
  }

  void TearDown() override
  {
    // Intentionally empty
  }

  std::shared_ptr<MockDataStore> mockDataStore{};
  std::shared_ptr<TestUiEventQueue> uiEventQueue{};
  std::shared_ptr<TestUiCommandQueue> uiCommandQueue{};

  std::unique_ptr<OutpostUiHandler> handler{};
};
} // namespace

TEST_F(Unit_Bsgalone_Client_Ui_Outpost_OutpostUiHandler, ThrowsWhenDataStoreIsNull)
{
  EXPECT_THROW([this]() { OutpostUiHandler(nullptr, uiEventQueue, uiCommandQueue); }(),
               std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Client_Ui_Outpost_OutpostUiHandler, ThrowsWhenEventQueueIsNull)
{
  EXPECT_THROW([this]() { OutpostUiHandler(mockDataStore, nullptr, uiCommandQueue); }(),
               std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Client_Ui_Outpost_OutpostUiHandler, ThrowsWhenCommandQueueIsNull)
{
  EXPECT_THROW([this]() { OutpostUiHandler(mockDataStore, uiEventQueue, nullptr); }(),
               std::invalid_argument);
}

TEST_F(Unit_Bsgalone_Client_Ui_Outpost_OutpostUiHandler, PublishesUndockCommandWhenButtonIsClicked)
{
  MockTexturePack pack;
  handler->initializeMenus(pge::Vec2i{1000, 1000}, pack);

  auto data      = generateInputForClickAt(pge::Vec2i{470, 35});
  auto processed = handler->processUserInput(data);

  EXPECT_TRUE(processed);
  EXPECT_EQ(1u, uiCommandQueue->messages().size());
  EXPECT_EQ(UiCommandType::UNDOCK_REQUESTED, uiCommandQueue->messages().at(0)->type());
}

TEST_F(Unit_Bsgalone_Client_Ui_Outpost_OutpostUiHandler, PublishesLogoutCommandWhenButtonIsClicked)
{
  MockTexturePack pack;
  handler->initializeMenus(pge::Vec2i{1000, 1000}, pack);

  auto data      = generateInputForClickAt(pge::Vec2i{668, 22});
  auto processed = handler->processUserInput(data);

  EXPECT_TRUE(processed);
  EXPECT_EQ(1u, uiCommandQueue->messages().size());
  EXPECT_EQ(UiCommandType::LOGOUT_REQUESTED, uiCommandQueue->messages().at(0)->type());
}

} // namespace bsgalone::client
