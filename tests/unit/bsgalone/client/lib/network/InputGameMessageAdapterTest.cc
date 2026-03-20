
#include "InputGameMessageAdapter.hh"
#include "SystemListMessage.hh"
#include "SystemsListReceivedEvent.hh"
#include "TestUiEventQueue.hh"
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::client {

TEST(Unit_Bsgalone_Client_Network_InputGameMessageAdapter,
     PublishesSystemsReceivedEventWhenSystemListIsReceived)
{
  auto queue = std::make_shared<TestUiEventQueue>();
  InputGameMessageAdapter adapter(queue);

  std::vector<core::System> systemsData{core::System{
                                          .dbId = core::Uuid{19},
                                          .name = "system1",
                                        },
                                        core::System{
                                          .dbId = core::Uuid{20},
                                          .name = "system2",
                                        }};
  core::SystemListMessage message(core::Uuid{18}, systemsData);
  adapter.onEventReceived(message);

  EXPECT_EQ(1u, queue->messages().size());
  const auto &actual = queue->messages().at(0);
  EXPECT_EQ(client::UiEventType::SYSTEMS_LIST_RECEIVED, actual->type());
  EXPECT_EQ(systemsData, actual->as<SystemsListReceivedEvent>().getSystems());
}

} // namespace bsgalone::client
