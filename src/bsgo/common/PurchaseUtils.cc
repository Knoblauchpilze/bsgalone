
#include "PurchaseUtils.hh"
#include <vector>

namespace bsgo {

namespace {
struct Cost
{
  Uuid resource{};
  int amount{};
};

bool playerHasEnoughResources(const std::vector<PlayerResourceData> &resources, const Cost &cost)
{
  for (const auto &resource : resources)
  {
    if (resource.dbId == cost.resource)
    {
      return resource.amount >= cost.amount;
    }
  }

  return false;
}

template<typename ItemType>
auto getItemPrice(const Uuid itemId, const std::vector<ItemType> &items) -> std::vector<Cost>
{
  std::vector<Cost> out{};

  const auto maybeItem = std::find_if(items.begin(), items.end(), [&itemId](const ItemType &item) {
    return item.dbId == itemId;
  });

  if (maybeItem != items.end())
  {
    for (const auto &[resourceId, cost] : maybeItem->price)
    {
      out.emplace_back(resourceId, cost);
    }
  }

  return out;
}
} // namespace

auto computeAffordability(const AffordabilityData &data) -> Affordability
{
  std::vector<Cost> costs{};
  switch (data.itemType)
  {
    case bsgalone::core::Item::WEAPON:
      costs = getItemPrice(data.itemId, data.weapons);
      break;
    case bsgalone::core::Item::COMPUTER:
      costs = getItemPrice(data.itemId, data.computers);
      break;
    case bsgalone::core::Item::SHIP:
      costs = getItemPrice(data.itemId, data.ships);
      break;
    default:
      throw std::invalid_argument("Failed to determine affordability of item with id "
                                  + str(data.itemId) + " neither a wepaon nor a computer");
  }

  Affordability out{.canAfford = true};

  for (const auto &cost : costs)
  {
    const auto enough                       = playerHasEnoughResources(data.playerResources, cost);
    out.resourceAvailibility[cost.resource] = enough;
    if (!enough)
    {
      out.canAfford = false;
    }
  }

  return out;
}

} // namespace bsgo
