
#pragma once

#include "AbstractRepository.hh"
#include "PlayerShip.hh"
#include "Tick.hh"
#include "Uuid.hh"
#include <memory>
#include <optional>
#include <set>
#include <unordered_set>
#include <vector>

namespace bsgalone::core {

class PlayerShipRepository : public AbstractRepository
{
  public:
  PlayerShipRepository(const DbConnectionShPtr &connection);
  ~PlayerShipRepository() override = default;

  void initialize() override;

  auto findOneById(const Uuid ship) const -> PlayerShip;
  auto findOneByPlayerAndActive(const Uuid player) const -> PlayerShip;
  auto findAllByPlayer(const Uuid player) const -> std::unordered_set<Uuid>;
  auto findAllAvailableWeaponSlotByShip(const Uuid ship) -> std::set<Uuid>;
  auto findAllBySystemAndRespawnTime(const Uuid systemDbId, const chrono::Tick &until)
    -> std::vector<PlayerShip>;

  void create(const PlayerShip &ship);
  void save(const PlayerShip &ship);
  void saveJump(const Uuid shipDbId, const std::optional<Uuid> jumpSystem);

  void saveRespawn(const Uuid ship, const chrono::Tick &death, const chrono::Tick &respawn);
  void deleteRespawn(const Uuid ship);

  private:
  auto fetchShipBase(const Uuid ship) const -> PlayerShip;
  void fetchSlots(const Uuid ship, PlayerShip &out) const;
  void fetchAiTargets(const Uuid ship, PlayerShip &out) const;
  void registerShipJump(const Uuid ship, const Uuid system) const;
  void cancelShipJump(const Uuid ship) const;
};

using PlayerShipRepositoryShPtr = std::shared_ptr<PlayerShipRepository>;

} // namespace bsgalone::core
