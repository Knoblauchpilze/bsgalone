
#include "PlayerMessagePublisher.hh"
#include "SystemData.hh"
#include "SystemListMessage.hh"

namespace bsgalone::core {

PlayerMessagePublisher::PlayerMessagePublisher(IMessageQueueShPtr outputMessageQueue)
  : m_outputMessageQueue(std::move(outputMessageQueue))
{
  if (m_outputMessageQueue == nullptr)
  {
    throw std::invalid_argument("Expected non null output message queue");
  }
}

namespace {
auto toSystemData(const System &system) -> SystemData
{
  return SystemData{
    .dbId     = system.dbId,
    .name     = system.name,
    .position = system.position,
  };
}
} // namespace

void PlayerMessagePublisher::publishSystemList(const Uuid playerDbId,
                                               const std::vector<System> &systems)
{
  if (systems.empty())
  {
    return;
  }

  std::vector<SystemData> systemsData{};
  std::transform(systems.begin(),
                 systems.end(),
                 std::back_inserter(systemsData),
                 [](const System &system) { return toSystemData(system); });

  // TODO: The system identifier should be fetched for the player
  auto out = std::make_unique<SystemListMessage>(playerDbId, Uuid{0}, systemsData);

  m_outputMessageQueue->pushEvent(std::move(out));
}

} // namespace bsgalone::core
