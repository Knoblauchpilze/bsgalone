#pragma once

#include "AbstractService.hh"
#include "AsteroidRepository.hh"
#include "PlayerRepository.hh"
#include <memory>
#include <vector>

namespace bsgo {

class LoadingService : public AbstractService
{
  public:
  LoadingService(const Repositories &repositories);
  ~LoadingService() override = default;

  auto getPlayersInSystem(const Uuid systemDbId) const -> std::vector<Player>;

  struct AsteroidProps
  {
    Uuid dbId{};
    Asteroid dbAsteroid{};

    std::optional<Uuid> resource{};
    std::optional<float> amount{};
  };
  auto getAsteroidsInSystem(const Uuid systemDbId) const -> std::vector<AsteroidProps>;
};

using LoadingServicePtr   = std::unique_ptr<LoadingService>;
using LoadingServiceShPtr = std::shared_ptr<LoadingService>;

} // namespace bsgo
