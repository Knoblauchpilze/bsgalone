
#include "OutpostUiHandler.hh"
#include "MockDataStore.hh"
#include "SynchronizedUiCommandQueue.hh"
#include "SynchronizedUiEventQueue.hh"
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
    uiEventQueue   = createSynchronizedUiEventQueue();
    uiCommandQueue = createSynchronizedUiCommandQueue();
    handler = std::make_unique<OutpostUiHandler>(mockDataStore, uiEventQueue, uiCommandQueue);
  }

  void TearDown() override
  {
    // Intentionally empty
  }

  std::shared_ptr<MockDataStore> mockDataStore{};
  IUiEventQueueShPtr uiEventQueue{};
  IUiCommandQueueShPtr uiCommandQueue{};

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

} // namespace bsgalone::client
