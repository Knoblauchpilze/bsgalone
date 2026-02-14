
#include "ServerView.hh"
#include "SystemListMessage.hh"

namespace pge {

ServerView::ServerView(GameSessionShPtr gameSession)
  : AbstractView("server", {bsgo::MessageType::SYSTEM_LIST})
  , m_gameSession(std::move(gameSession))
{
  if (nullptr == m_gameSession)
  {
    error("Expected non null game session");
  }
}

bool ServerView::isReady() const noexcept
{
  return m_gameSession->hasSystemDbId() && !m_systems.empty();
}

void ServerView::reset()
{
  m_systems.clear();
}

auto ServerView::getPlayerSystem() const -> bsgo::Uuid
{
  return m_gameSession->getSystemDbId();
}

auto ServerView::getPlayerSystemName() const -> std::string
{
  const auto playerSystemId = getPlayerSystem();
  const auto maybeSystem    = std::find_if(m_systems.begin(),
                                        m_systems.end(),
                                        [&playerSystemId](const bsgo::SystemData &system) {
                                          return system.dbId == playerSystemId;
                                        });

  if (maybeSystem == m_systems.end())
  {
    error("Failed to get system name",
          "No data received from server for system " + bsgo::str(playerSystemId));
  }

  return maybeSystem->name;
}

auto ServerView::getAllSystems() const -> std::vector<bsgo::SystemData>
{
  if (m_systems.empty())
  {
    error("No systems available");
  }

  return m_systems;
}

namespace {
auto initializeBounds() -> ServerView::Bounds
{
  ServerView::Bounds out;

  constexpr auto minValue = std::numeric_limits<float>::max();
  out.min                 = Eigen::Vector3f(minValue, minValue, minValue);

  constexpr auto maxValue = std::numeric_limits<float>::lowest();
  out.max                 = Eigen::Vector3f(maxValue, maxValue, maxValue);

  return out;
}

void updateMinFromValue(Eigen::Vector3f &toUpdate, const Eigen::Vector3f value)
{
  toUpdate(0) = std::min(toUpdate(0), value(0));
  toUpdate(1) = std::min(toUpdate(1), value(1));
  toUpdate(2) = std::min(toUpdate(2), value(2));
}

void updateMaxFromValue(Eigen::Vector3f &toUpdate, const Eigen::Vector3f value)
{
  toUpdate(0) = std::max(toUpdate(0), value(0));
  toUpdate(1) = std::max(toUpdate(1), value(1));
  toUpdate(2) = std::max(toUpdate(2), value(2));
}
} // namespace

auto ServerView::getMapBounds() const -> Bounds
{
  const auto systems = getAllSystems();

  auto out = initializeBounds();

  for (const auto &system : systems)
  {
    updateMinFromValue(out.min, system.position);
    updateMaxFromValue(out.max, system.position);
  }

  return out;
}

void ServerView::handleMessageInternal(const bsgalone::core::IMessage &message)
{
  const auto systemList = message.as<bsgo::SystemListMessage>();
  m_systems             = systemList.getSystemsData();
}

} // namespace pge
