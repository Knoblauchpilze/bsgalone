
#include "PurchaseUtils.hh"
#include "PlayerResourceRepository.hh"
#include <vector>

namespace bsgo {

namespace {
struct Cost
{
  Uuid resource{};
  int amount{};
};

bool playerHasEnoughResources(const std::vector<PlayerResource> &resources, const Cost &cost)
{
  for (const auto &resource : resources)
  {
    if (resource.resource == cost.resource)
    {
      return resource.amount >= cost.amount;
    }
  }

  return false;
}

auto getWeaponPrice(const Uuid &weaponId, const WeaponPriceRepository &repository)
  -> std::vector<Cost>
{
  std::vector<Cost> out{};

  const auto price = repository.findAllByWeapon(weaponId);
  for (const auto &[resourceId, cost] : price)
  {
    out.emplace_back(resourceId, cost);
  }

  return out;
}

auto getComputerPrice(const Uuid &computerId, const ComputerPriceRepository &repository)
  -> std::vector<Cost>
{
  std::vector<Cost> out{};

  const auto price = repository.findAllByComputer(computerId);
  for (const auto &[resourceId, cost] : price)
  {
    out.emplace_back(resourceId, cost);
  }

  return out;
}

auto getShipPrice(const Uuid &shipId, const ShipPriceRepository &repository) -> std::vector<Cost>
{
  std::vector<Cost> out{};

  const auto price = repository.findAllByShip(shipId);
  for (const auto &[resourceId, cost] : price)
  {
    out.emplace_back(resourceId, cost);
  }

  return out;
}
} // namespace

auto computeAffordability(const AffordabilityData &data) -> Affordability
{
  std::vector<Cost> costs{};
  switch (data.itemType)
  {
    case Item::WEAPON:
      costs = getWeaponPrice(data.itemId, *data.weaponPriceRepo);
      break;
    case Item::COMPUTER:
      costs = getComputerPrice(data.itemId, *data.computerPriceRepo);
      break;
    case Item::SHIP:
      costs = getShipPrice(data.itemId, *data.shipPriceRepo);
      break;
    default:
      throw std::invalid_argument("Failed to determine affordability of item with id "
                                  + str(data.itemId) + " neither a wepaon nor a computer");
  }

  const auto playerResources = data.resourceRepo->findAllByPlayer(data.playerId);
  Affordability out{.canAfford = true};

  for (const auto &cost : costs)
  {
    const auto enough                       = playerHasEnoughResources(playerResources, cost);
    out.resourceAvailibility[cost.resource] = enough;
    if (!enough)
    {
      out.canAfford = false;
    }
  }

  return out;
}

} // namespace bsgo
