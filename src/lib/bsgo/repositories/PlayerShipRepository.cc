
#include "PlayerShipRepository.hh"

namespace bsgo {

PlayerShipRepository::PlayerShipRepository(const DbConnectionShPtr &connection)
  : AbstractRepository("player", connection)
{
  addModule("ship");
}

auto PlayerShipRepository::findOneById(const Uuid &ship) const -> PlayerShip
{
  PlayerShip out;

  switch (ship)
  {
    case Uuid{0}:
      out.faction = Faction::COLONIAL;
      out.player  = {Uuid{0}};
      out.name    = "Viper mark II";
      out.active  = true;

      out.hullPoints      = 369.2f;
      out.maxHullPoints   = 585.0f;
      out.hullPointsRegen = 0.5f;

      out.powerPoints    = 11.9f;
      out.maxPowerPoints = 150.0f;
      out.powerRegen     = 2.0f;

      out.acceleration = 5.0f;
      out.speed        = 4.0f;

      out.radius   = 0.5f;
      out.position = Eigen::Vector3f::Zero();
      break;
    case Uuid{1}:
      out.faction = Faction::CYLON;
      out.name    = "Cylon raider";
      out.active  = true;

      out.hullPoints      = 280.0f;
      out.maxHullPoints   = 300.0f;
      out.hullPointsRegen = 1.0f;

      out.powerPoints    = 23.2f;
      out.maxPowerPoints = 75.0f;
      out.powerRegen     = 5.0f;

      out.acceleration = 7.0f;
      out.speed        = 6.0f;

      out.radius   = 0.7f;
      out.position = Eigen::Vector3f(2.0f, 5.0f, 0.0f);
      break;
    case Uuid{2}:
      out.faction = Faction::COLONIAL;
      out.player  = {Uuid{0}};
      out.name    = "Jotunn";
      out.active  = false;

      out.hullPoints      = 3000.0f;
      out.maxHullPoints   = 3000.0f;
      out.hullPointsRegen = 50.0f;

      out.powerPoints    = 750.0f;
      out.maxPowerPoints = 750.0f;
      out.powerRegen     = 32.0f;

      out.acceleration = 1.0f;
      out.speed        = 2.0f;

      out.radius   = 2.0f;
      out.position = Eigen::Vector3f(-1.0f, 0.0f, 0.0f);
      break;
    default:
      error("Ship for player " + str(ship) + " not found");
      break;
  }

  return out;
}

auto PlayerShipRepository::findAllByPlayer(const Uuid &player) const -> std::vector<Uuid>
{
  if (player != Uuid{0})
  {
    error("Player " + str(player) + " not found");
  }
  return {Uuid{0}, Uuid{2}};
}

} // namespace bsgo
