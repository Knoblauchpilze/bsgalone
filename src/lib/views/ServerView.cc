
#include "ServerView.hh"

namespace bsgo {

ServerView::ServerView(const CoordinatorShPtr &coordinator, const Repositories &repositories)
  : IView("server", coordinator, repositories)
{}

auto ServerView::getAllSystems() const -> std::vector<System>
{
  std::vector<System> out;

  const auto ids = m_repositories.systemRepository->findAll();
  for (const auto &id : ids)
  {
    auto system = m_repositories.systemRepository->findOneById(id);
    out.push_back(std::move(system));
  }

  return out;
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

} // namespace bsgo
